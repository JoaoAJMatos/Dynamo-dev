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
    Blockchain();

    /**
     * @brief Construct a new Blockchain object
     * 
     * @param isRoot - Indicates if a new Blockchain should be created or not
     */
    Blockchain(int isRoot, const std::string& firstNodeAddress);

    explicit Blockchain(std::string blockchain_packet); // Build a Blockchain class from an incomming messagepack

    /* PUBLIC FUNCTIONS */
    /**
     * @brief Replaces the current _chain by an incomming one if the contents are valid
     * 
     * @return int 
     */
    int replaceChain(const Blockchain& _chain);

    /**
     * @brief Adds a new block to the chain by mining
     * 
     * @param data 
     * @param log 
     * @return int 
     */
    int addBlock(std::vector<Transaction*> data, int log);

    /**
     * @brief Validates the transactions inside the blockchain
     * 
     * @param blockchain
     * @return int 
     */
    int isTransactionDataValid(Blockchain blockchain);

    Block getLastBlock();

    /**
     * @brief Validates the chain contents
     * 
     * @param chain 
     * @return int 
     */
    static int isValid(Blockchain chain);

    static std::string toString(const Blockchain& chain);

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