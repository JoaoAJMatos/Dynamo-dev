#ifndef DEV_DYNAMO_BLOCKCHAIN_H
#define DEV_DYNAMO_BLOCKCHAIN_H

#include "../block/Block.h"
#include "../wallet/transaction/Transaction.h"
#include "../consensus/consensus.hpp"
#include <vector>

class Blockchain
{
private:
    /* MEMBER VARIABLES */
    std::vector<Block*> chain;

public:
    /* CONSTRUCTOR */
    /**
     * @brief Construct a new Blockchain object
     * 
     * @param isRoot - Indicates if a new Blockchain should be created or not
     */
    Blockchain(int isRoot, const std::string& firstNodeAddress);

    /* PUBLIC FUNCTIONS */
    /**
     * @brief Replaces the current chain by an incomming one if the contents are valid
     * 
     * @return int 
     */
    int replaceChain(Blockchain chain);

    /**
     * @brief Adds a new block to the chain by mining
     * 
     * @param data 
     * @param log 
     * @return int 
     */
    int addBlock(std::vector<Transaction> data, int log);

    /**
     * @brief Validates the transactions inside the chain
     * 
     * @param chain 
     * @return int 
     */
    int isTransactionDataValid(Blockchain chain);

    /**
     * @brief Validates the chain contents
     * 
     * @param chain 
     * @return int 
     */
    int isValid(Blockchain chain);

    /**
     * @brief Prints the chain to the std out
     * 
     */
    void printChain();

    /**
     * @brief Prints a block at a specified index
     * 
     * @param height 
     */
    void printBlock(int height);

    /* GETTERS */
    std::vector<Block*> getChain();
};

#endif