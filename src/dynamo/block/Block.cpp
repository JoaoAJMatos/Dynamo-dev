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

    uint8_t target[32];
    memset(target, 0, sizeof(target));
    target[2] = 0xFF;

    // This buffer will contain the block contents that will be used to make the block's hash
    std::string dataToHash;
    for (auto& dataElement : data)
    {
        dataToHash.append(dataElement);
    }

    do // POW
    {
        std::stringstream blockBuffer;
        nonce++;
        timestamp = Time::getTimestamp();
        difficulty = Block::adjustDifficulty(lastBlock, timestamp);
        
        // Make the block buffer to hash
        blockBuffer << timestamp << height << lastHash << dataToHash << difficulty << nonce << std::endl;

        sha.update(blockBuffer.str().data());
        hash = sha.digest();

        if (log) std::cout  << "Hash: " << sha.toString(hash) << " | Nonce: " << nonce << std::endl;

    } while (memcmp(target, hash, sizeof(target)) < 0);

    std::cout << "[+] Nonce found for block [" << height << "]" << std::endl;

    return new Block(timestamp, hash, lastHash, height, nonce, difficulty, data);
}

int Block::adjustDifficulty(Block lastBlock, std::time_t timestamp)
{
    const int difficulty = lastBlock.difficulty;

    if (difficulty < 1) return 1;
    if ((timestamp - lastBlock.timestamp) > MINE_RATE) return difficulty - 1;

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