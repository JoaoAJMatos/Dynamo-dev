//
// Created by joaoa on 21/04/2022.
//

#ifndef DEV_DYNAMO_BLOCK_H
#define DEV_DYNAMO_BLOCK_H

#define MINE_RATE 1000

#include <ctime>
#include <iostream>
#include <vector>
#include <sstream>
#include <cstring>
#include <math.h>
#include <cstdio>

#include "../../crypto/SHA-2/SHA256.h"
#include "../../system/time/Time.h"

class Block
{
private:

    /* MEMBER VARIABLES */
    std::time_t timestamp;
    uint8_t* hash;
    uint8_t* prev_hash;
    size_t height;
    size_t nonce;
    int difficulty;

    std::vector<std::string> data; // Array of transactions

    SHA256 sha;
    Time t;

public:
    /* CONSTRUCTOR */
    /**
     * @brief Construct a new Block object
     * 
     * @param timestamp 
     * @param hash 
     * @param prev_hash 
     * @param height 
     * @param nonce 
     * @param difficulty 
     */
    Block(std::time_t timestamp, uint8_t* hash, uint8_t* prev_hash, size_t height, size_t nonce, int difficulty, std::vector<std::string> data);

    /* PUBLIC FUNCTIONS */
    /**
     * @brief Performs the POW to mine a block
     * 
     * @param lastBlock  - A pointer to the previous block
     * @param log        - Indicates if the mining process should be logged to the standard out
     * 
     * @return Block* 
     */
    static Block* mineBlock(Block lastBlock, std::vector<std::string> data, int log);

    /**
     * @brief Adjusts the block mining difficulty in order to keep the mining rate between the defined boundries
     * 
     * @param lastBlock 
     * @param timestamp 
     * @return int 
     */
    static int adjustDifficulty(Block lastBlock, std::time_t timestamp);

    /**
     * @brief Prints the block's contentes to the standard-out
     * 
     */
    void printBlock();
};


#endif //DEV_DYNAMO_BLOCK_H
