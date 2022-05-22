#ifndef DEV_DYNAMO_BLOCKCHAIN_H
#define DEV_DYNAMO_BLOCKCHAIN_H

#define REWARD_TO_ROOT 2000000000 // The reward of 2.000.000.000 Dynamos 
                                  // will be given to the first person that joins the network

#include "../block/Block.h"
#include <vector>

class Blockchain
{
private:
    /* MEMBER VARIABLES */
    std::vector<Block> chain;

    Block* makeGenesis();

public:
    /* CONSTRUCTOR */

    /**
     * @brief Construct a new Blockchain object
     * 
     * @param isRoot - Indicates if a new Blockchain should be created or not
     */
    Blockchain(bool isRoot);

    /* PUBLIC FUNCTIONS */

    /**
     * @brief Replaces the current chain by an incomming one if the contents are valid
     * 
     * @return int 
     */
    int replaceChain();

    int addBlock();
};

#endif