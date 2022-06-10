#ifndef DEV_DYNAMO_BLOCKCHAIN_H
#define DEV_DYNAMO_BLOCKCHAIN_H

#include "../block/Block.h"
#include "../wallet/transaction/Transaction.h"
#include "../../../libs/msgpack11/msgpack11.hpp"
#include "../consensus/consensus.hpp"
#include "../../crypto/EC/ECDSA.h"

#include <vector>
#include <map>
#include <sstream>

class Blockchain
{
public:
    std::vector<Block*> chain;

    /* CONSTRUCTOR */
    /**
     * @brief Construct a new Blockchain object
     * 
     * @param isRoot - Indicates if a new Blockchain should be created or not
     */
    Blockchain(int isRoot, const std::string& firstNodeAddress);

    Blockchain(std::string blockchain_packet); // Build a Blockchain class from an incomming messagepack 

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

    static msgpack11::MsgPack serialize(Blockchain chain);

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

    /**
     * @brief Calculates the balance of a specified address
     * 
     * @param address 
     * @return int 
     */
    int calculateBalanceOfAddress(const std::string& address);
};

#endif