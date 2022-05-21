//
// Created by joaoa on 21/04/2022.
//

#ifndef DEV_DYNAMO_BLOCK_H
#define DEV_DYNAMO_BLOCK_H

#include <ctime>
#include <iostream>
#include <vector>

#include "../wallet/transaction/Transaction.h"

class Block
{
private:

    /* MEMBER VARIABLES */
    std::time_t timestamp;
    std::string hash;
    std::string prev_hash;
    size_t height;
    size_t nonce;
    int difficulty;

    std::vector<Transaction> data; // Array of transactions


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
    Block(std::time_t timestamp, std::string hash, std::string prev_hash, size_t height, size_t nonce, int difficulty);

    /* PUBLIC FUNCTIONS */
    /**
     * @brief Performs the POW to mine a block
     * 
     * @param lastBlock  - A pointer to the previous block
     * @param log        - Indicates if the mining process should be logged to the standard out
     * 
     * @return Block* 
     */
    static Block* mineBlock(Block* lastBlock, int log);

    /**
     * @brief Adjusts the block mining difficulty in order to keep the mining rate between the defined boundries
     * 
     * @param lastBlock 
     * @param timestamp 
     * @return int 
     */
    static int adjustDifficulty(Block* lastBlock, std::time_t timestamp);

    /**
     * @brief Prints the block's contentes to the standard-out
     * 
     */
    void printBlock();
};


#endif //DEV_DYNAMO_BLOCK_H
