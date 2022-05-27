//
// Created by joaoa on 21/04/2022.
//

#include "Block.h"

/**
 * @brief Construct a new Block:: Block object
 * 
 * @param timestamp 
 * @param hash 
 * @param prev_hash 
 * @param height 
 * @param nonce 
 * @param difficulty 
 */
Block::Block(std::time_t timestamp, uint8_t* hash, uint8_t* prev_hash, size_t height, size_t nonce, int difficulty, std::vector<Transaction> data)
{
    this->timestamp = timestamp;
    this->hash = hash;
    this->prev_hash = prev_hash;
    this->height = height;
    this->nonce = nonce;
    this->difficulty = difficulty;
    this->data = data;
}

/* HELPER FUNCTION */
/**
 * @brief Sets a target buffer for the proof of work
 * 
 * @param destinationBuffer 
 * @param difficulty 
 * @return int 
 * 
 * @details This function creates a uint8_t[32] target buffer for the proof of work.
 *          It will fill in the required number of zeros to complete the POW.
 */
int setTarget(uint8_t* destinationBuffer, int difficulty)
{

    // Devide the difficulty by the number of octets in the buffer
    // The integer part of the resulting division will correspond to the amount of octets that should be filled with zeros.
    // The remainder part indicates how many left bits should be filled with zeros in the next octet (kinda)
    //
    // Example:
    //  - For a difficulty of 13 bits, the resulting division will be 1.625. By filling the first octet of the buffer + 6
    //    left bits on the second octet we get a total number of 14 bits (8 + 6), which is not what we are looking for. To
    //    get the actual number of left bits we must subtract 1 to the amount of left bits indicated by the remainder. 
    
    int div = difficulty / 8; // Get the integer part of the division
    int rem = difficulty % 8; // Get the remainder part

    memset(destinationBuffer, 0, div);    // Fill the amount of octets needed with zeros

    destinationBuffer[div] = 0xFF >> rem; // Set the octets bits to 1 and right shift them by the desired amount of zeros.
                                          // The rest of the buffer can be left with random data since memcmp returns < 0
                                          // when the first byte that does not match in both memory blocks has a lower value
                                          // in ptr1 than in ptr2

    return 0;
}

/**
 * @brief 
 * 
 * @param lastBlock 
 * @param log 
 * @return Block* 
 * 
 * @details This function performs the proof of work required to mine a block
 */
Block* Block::mineBlock(Block lastBlock, std::vector<Transaction> data, int log)
{
    uint8_t* hash;
    std::time_t timestamp;
    size_t height = lastBlock.height + 1;
    uint8_t* lastHash = lastBlock.hash;
    int difficulty = lastBlock.difficulty;
    int newDifficulty;
    size_t nonce = 0;

    SHA256 sha;
    Time t;

    uint8_t target[32];
    
    // This buffer will contain the block contents that will be used to make the block's hash
    std::string dataToHash;
    for (auto& transaction : data)
    {
        dataToHash.append(transaction.getTransactionDataBuffer());
    }

    t.startHighResClock();

    do // POW
    {
        std::stringstream blockBuffer;
        nonce++;
        timestamp = Time::getTimestamp();
        newDifficulty = Block::adjustDifficulty(lastBlock, timestamp);

        // Make the target buffer
        setTarget(target, difficulty);

        // Make the block buffer to hash
        blockBuffer << timestamp << height << lastHash << dataToHash << difficulty << nonce << std::endl;

        sha.update(blockBuffer.str());
        hash = sha.digest();

        if (log) std::cout  << "\r" << "Hash: " << SHA256::toString(hash) << " | Nonce: " << nonce << std::flush;

    } while (memcmp(target, hash, sizeof(target)) < 0);

    t.stopHighResClock();

    std::cout << std::endl << "[+] Nonce found for block [" << height << "] | Mining time: " << t.getTimeElapsed() << " seconds" << std::endl;

    return new Block(timestamp, hash, lastHash, height, nonce, difficulty, data);
}

int Block::adjustDifficulty(Block lastBlock, std::time_t timestamp)
{
    const int difficulty = lastBlock.difficulty;

    if (difficulty < 1) return 1;
    if ((timestamp - lastBlock.timestamp) > MINE_RATE) return difficulty - 1;
    if (difficulty == 64) return 64;

    return difficulty + 1;
}

/**
 * @brief Returns the Genesis Block
 * 
 * @return Block* 
 * 
 * @details The genesis block sends a special transaction to the first node that joins the network and initiates the chain.
 */
Block* Block::genesis(const std::string& first_node_wallet_address, int reward)
{
    SHA256 sha;

    std::time_t timestamp = Time::getTimestamp();
    sha.update("dynamo");
    uint8_t* hash = sha.digest();

    std::string sender = "Genesis";

    // Create transaction data
    std::vector<Transaction> data;
    data.push_back(Transaction(nullptr, first_node_wallet_address, reward, sender, reward));

    return new Block(timestamp, hash, hash, 0, 0, INITIAL_DIFFICULTY, data);
}

/**
 * @brief This function prints the blocks content to the standard out
 * 
 */
void Block::printBlock()
{
    std::cout << "Block " << height << std::endl;
    std::cout << "{" << std::endl;
    std::cout << "  Timestamp: " << this->timestamp << std::endl;
    std::cout << "  Hash: " << SHA256::toString(this->hash) << std::endl;
    std::cout << "  Previous hash: " << SHA256::toString(this->prev_hash) << std::endl;
    std::cout << "  Height: " << this->height << std::endl;
    std::cout << "  Nonce: " << this->nonce << std::endl;
    std::cout << "  Difficulty: " << this->difficulty << std::endl;
    std::cout << "  Data:" << std::endl;
    std::cout << "  {" << std::endl;

    for (auto& transaction : this->data)
    {
        transaction.showTransaction();
    }

    std::cout << "  }" << std::endl;
    std::cout << "}" << std::endl;
}

std::vector<Transaction> Block::getData()
{
    return this->data;
}