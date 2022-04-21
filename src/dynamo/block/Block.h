//
// Created by joaoa on 21/04/2022.
//

#ifndef DEV_DYNAMO_BLOCK_H
#define DEV_DYNAMO_BLOCK_H

#include <ctime>
#include <iostream>

class Block
{
private:

    /* MEMBER VARIABLES */
    struct block_headers
    {
        std::time_t timestamp;
        std::string hash;
        std::string prev_hash;
        unsigned long long int nonce;
    };



public:

};


#endif //DEV_DYNAMO_BLOCK_H
