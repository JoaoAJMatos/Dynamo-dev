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
Block::Block(std::time_t timestamp, uint8_t* hash, uint8_t* prev_hash, size_t height, size_t nonce, int difficulty, std::vector<std::string> data)
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
    double division, fullOctets, remainder, leftBits;

    memset(destinationBuffer, 1, sizeof(destinationBuffer));

    // Devide the difficulty by the number of octets in the buffer
    // The integer part of the resulting division will correspond to the amount of octets that should be filled with zeros.
    // The remainder part indicates how many left bits should be filled with zeros in the next octet (kinda)
    //
    // Example:
    //  - For a difficulty of 13 bits, the resulting division will be 1.625. By filling the first octet of the buffer + 6
    //    left bits on the second octet we get a total number of 14 bits (8 + 6), which is not what we are looking for. To
    //    get the actual number of left bits we must subtract 1 to the amount of left bits indicated by the remainder. 
    division = (double)difficulty / 8;

    remainder = modf(division, &fullOctets); // The remainder will now hold 0.625, and the intpart will hold 1.
                                             // To get the amount of left bits needed we can multiply the remainder by 10 and repeat the operation. Then subtract 1
    
    modf(remainder*10, &leftBits);
    leftBits -= 1;

    // Fill in the destination buffer's first X octets with zeros
    memset(destinationBuffer, 0, (int)fullOctets);

    // Right shift the next octet (fullOctets + 1) by the amount of leading zeros needed 
    destinationBuffer[(int)fullOctets+1] >> (int)leftBits;

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
Block* Block::mineBlock(Block lastBlock, std::vector<std::string> data, int log)
{
    uint8_t* hash;
    std::time_t timestamp;
    size_t height = lastBlock.height + 1;
    uint8_t* lastHash = lastBlock.hash;
    int difficulty = lastBlock.difficulty;
    size_t nonce = 0;

    SHA256 sha;
    Time t;

    uint8_t target[32];
    
    // This buffer will contain the block contents that will be used to make the block's hash
    std::string dataToHash;
    for (auto& dataElement : data)
    {
        dataToHash.append(dataElement);
    }

    t.startHighResClock();

    do // POW
    {
        std::stringstream blockBuffer;
        nonce++;
        timestamp = Time::getTimestamp();
        difficulty = Block::adjustDifficulty(lastBlock, timestamp);

        // Make the target buffer
        setTarget(target, difficulty);

        // Make the block buffer to hash
        blockBuffer << timestamp << height << lastHash << dataToHash << difficulty << nonce << std::endl;

        sha.update(blockBuffer.str().data());
        hash = sha.digest();

        if (log) std::cout  << "\r" << "Hash: " << sha.toString(hash) << " | Nonce: " << nonce << std::flush;

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
 * @brief This function prints the blocks content to the standard out
 * 
 */
void Block::printBlock()
{
    std::cout << "Block " << height << std::endl;
    std::cout << "{" << std::endl;
    std::cout << "  Timestamp: " << this->timestamp << std::endl;
    std::cout << "  Hash: " << sha.toString(this->hash) << std::endl;
    std::cout << "  Previous hash: " << sha.toString(this->prev_hash) << std::endl;
    std::cout << "  Height: " << this->height << std::endl;
    std::cout << "  Nonce: " << this->nonce << std::endl;
    std::cout << "  Difficulty: " << this->difficulty << std::endl;
    std::cout << "  Data:" << std::endl;
    std::cout << "  {" << std::endl;

    for (auto& dataElement : this->data)
    {
        std::cout << "      " << dataElement << std::endl;
    }

    std::cout << "  }" << std::endl;
    std::cout << "}" << std::endl;
}