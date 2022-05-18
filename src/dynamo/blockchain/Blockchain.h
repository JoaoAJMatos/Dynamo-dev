#ifndef DEV_DYNAMO_BLOCKCHAIN_H
#define DEV_DYNAMO_BLOCKCHAIN_H

#include "../block/Block.h"
#include <vector>

class Blockchain
{
private:
    /* MEMBER VARIABLES */
    std::vector<Block> chain;

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