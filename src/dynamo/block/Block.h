//
// Created by joaoa on 21/04/2022.
//

#ifndef DEV_DYNAMO_BLOCK_H
#define DEV_DYNAMO_BLOCK_H

#include <ctime>
#include <iostream>
#include <vector>
#include <sstream>
#include <cstring>
#include <math.h>
#include <cstdio>

#include "../../crypto/SHA-2/SHA256.h"
#include "../../system/time/Time.h"
#include "../wallet/transaction/Transaction.h"
#include "../../../libs/msgpack11/msgpack11.hpp"

class Block
{
private:

    /* MEMBER VARIABLES */
    std::time_t timestamp;
    uint8_t* hash;
    uint8_t* prev_hash;
    std::string hashString;
    std::string prevHashString;
    size_t height;
    size_t nonce;
    int difficulty;

    SHA256 sha;
    Time t;

    static std::string toString(uint8_t* str);

public:
    std::vector<Transaction> data; // Array of transactions

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
    Block(std::time_t timestamp, uint8_t* hash, uint8_t* prev_hash, size_t height, size_t nonce, int difficulty, std::vector<Transaction> data);
    
    Block(std::string block_packet);

    /* STATIC FUNCTIONS */
    /**
     * @brief Performs the POW to mine a block
     * 
     * @param lastBlock  - A pointer to the previous block
     * @param log        - Indicates if the mining process should be logged to the standard out
     * 
     * @return Block* 
     */
    static Block* mineBlock(Block* lastBlock, std::vector<Transaction> data, int log);

    /**
     * @brief Adjusts the block mining difficulty in order to keep the mining rate between the defined boundries
     * 
     * @param lastBlock 
     * @param timestamp 
     * @return int 
     */
    static int adjustDifficulty(Block* lastBlock, std::time_t timestamp);

    /**
     * @brief Returns the genesis block
     * 
     * @return Block* 
     */
    static Block* genesis(const std::string& first_node_wallet_address, int reward);

    /**
     * @brief Serializes a block into a msgpack object
     * 
     * @param block 
     * @return msgpack11::MsgPack 
     */
    static msgpack11::MsgPack serialize(Block* block);

    /* PUBLIC FUNCTIONS */
    /**
     * @brief Prints the block's contentes to the standard-out
     * 
     */
    void printBlock();

    /* GETTERS */
    std::time_t getTimestamp() const;
    uint8_t* getHash();
    uint8_t* getPrevHash();
    size_t getHeight();
    size_t getNonce();
    int getDifficulty();
    std::vector<Transaction>* getData();
};


#endif //DEV_DYNAMO_BLOCK_H
