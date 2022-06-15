//
// Created by joaoa on 21/04/2022.
//

#include "Block.h"

uint8_t* hexToUint(const char* buffer)
{
    if (buffer == nullptr) return nullptr;

    size_t slength = strlen(buffer);

    if ((slength % 2) != 0) return nullptr; // must be even

    size_t dlength = slength / 2;

    uint8_t* data = (uint8_t*)malloc(dlength);

    memset(data, 0, dlength);

    size_t index = 0;
    while (index < slength) 
    {
        char c = buffer[index];
        int value = 0;
        
        if (c >= '0' && c <= '9')
            value = (c - '0');
        else if (c >= 'A' && c <= 'F')
            value = (10 + (c - 'A'));
        else if (c >= 'a' && c <= 'f')
            value = (10 + (c - 'a'));
        else
            return nullptr;

        data[(index / 2)] += value << (((index + 1) % 2) * 4);

        index++;
    }   

    return data;
}

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

Block::Block(std::string block_packet)
{
    using namespace msgpack11;

    // Deserialize the block packet
    std::string err;
    MsgPack block = MsgPack::parse(block_packet, err);

    try
    {
        std::cout << "[BLOCK HASH] " << block["hash"].string_value() << std::endl;

        this->timestamp = (time_t)block["timestamp"].int_value();
        this->hash = hexToUint(block["hash"].string_value().c_str());        // I know this might look like a mess,
        this->prev_hash = hexToUint(block["prev_hash"].string_value().c_str());   // but it works
        this->height = (size_t)block["height"].int_value();
        this->nonce = (size_t)block["nonce"].int_value();
        this->difficulty = (size_t)block["difficulty"].int_value();

        std::vector<Transaction> dataArray;

        for (int i = 0; i < block["data"].array_items().size(); i++)
        {
            Transaction transact(block["data"][i].dump());
            dataArray.push_back(transact);
        }

        this->data = dataArray;
    }
    catch(const std::exception& e)
    {
        std::cout << "[ERROR] Unable to create a block instance from the incomming packet." << std::endl;
    }
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
Block* Block::mineBlock(Block* lastBlock, std::vector<Transaction> data, int log)
{
    uint8_t* hash;
    std::time_t timestamp;
    size_t height = lastBlock->height + 1;
    uint8_t* lastHash = lastBlock->hash;
    int difficulty = lastBlock->difficulty;
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

    // These variables will be used to calculate the mining power of the node
    std::time_t hashCountStart = Time::getTimestamp();
    int hashCount = 0;
    std::stringstream blockBuffer;

    do // POW
    {
        blockBuffer.clear();
        nonce++;
        hashCount++;
        timestamp = Time::getTimestamp();
        newDifficulty = Block::adjustDifficulty(lastBlock, timestamp);

        // Make the target buffer
        setTarget(target, difficulty);

        // Make the block buffer to hash
        blockBuffer << timestamp << height << lastHash << dataToHash << difficulty << nonce << std::endl;

        sha.update(blockBuffer.str());
        hash = sha.digest();

        if (log)
        {
            if ((Time::getTimestamp() - hashCountStart) >= 1)
            {
                hashCountStart = Time::getTimestamp();
                hashCount = 0;
            }

            std::cout  << "\r" << "Hash: " << SHA256::toString(hash) << " | Nonce: " << nonce << " | Hash rate (hashes/s): " << hashCount << std::flush;
        }

    } while (memcmp(target, hash, sizeof(target)) < 0);

    t.stopHighResClock();

    std::cout << std::endl << "[+] Nonce found for block [" << height << "] | Mining time: " << t.getTimeElapsed() << " seconds" << std::endl;

    return new Block(timestamp, hash, lastHash, height, nonce, difficulty, data);
}

int Block::adjustDifficulty(Block* lastBlock, std::time_t timestamp)
{
    const int difficulty = lastBlock->difficulty;

    if (difficulty < 1) return 1;
    if ((timestamp - lastBlock->timestamp) > MINE_RATE) return difficulty - 1;
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

std::string Block::toString(uint8_t* str)
{
    return std::string((char *)str);
}

msgpack11::MsgPack Block::serialize(Block* block)
{
    using namespace msgpack11;

    MsgPack data;
    MsgPack::array tempData;

    for (auto& transaction : block->data)
    {
        tempData.push_back(Transaction::serialize(&transaction));
    }

    data = tempData;

    MsgPack tempBlock = MsgPack::object {
        {"timestamp", (int)block->getTimestamp()},
        {"hash", Block::toString(block->getHash())},
        {"prev_hash", Block::toString(block->getPrevHash())},
        {"height", (int)block->getHeight()},
        {"nonce", (int)block->getNonce()},
        {"difficulty", (int)block->getDifficulty()},
        {"data", data}
    };

    return tempBlock;
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

std::vector<Transaction>* Block::getData()
{
    return &this->data;
}

size_t Block::getHeight()
{
    return this->height;
}

uint8_t* Block::getHash()
{
    return this->hash;
}

uint8_t* Block::getPrevHash()
{
    return this->prev_hash;
}

std::time_t Block::getTimestamp() const
{
    return this->timestamp;
}

size_t Block::getNonce()
{
    return this->nonce;
}

int Block::getDifficulty()
{
    return this->difficulty;
}