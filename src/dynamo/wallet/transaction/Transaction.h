//
// Created by Matos on 4/22/2022.
//

#ifndef DEV_DYNAMO_TRANSACTION_H
#define DEV_DYNAMO_TRANSACTION_H

#include <string>

// TODO: fix this include path
//#include "../../../src/util/uuid/uuid.h"

class Transaction
{
private:
    /* MEMBER VARIABLES */
    char id[38];

    // The output map contains information about the sender and the recipient
    typedef struct outputMap
    {
        std::string recipient; // The recipients address
        int amount;            // The amount to be transacted

        std::string sender;    // The senders address
        int balance;           // The senders balance after the transaction
    };

    typedef struct inputMap
    {
        
    };
    

    /* PRIVATE FUNCTIONS */
    


public:
    /* CONSTRUCTOR */
    Transaction();

    /* PUBLIC FUNCTIONS */
    /**
     * @brief Updates transactions and resigns them
     * 
     * @return int 
     */
    int update();


    /* STATIC FUNCTIONS */
    /**
     * @brief Creates a reward transaction
     * 
     * @return this 
     */
    static Transaction rewardTransaction();

    /**
     * @brief Validates a transaction
     * 
     * @param transaction 
     * @return int 
     */
    static int validTransaction(Transaction transaction);
};


#endif //DEV_DYNAMO_TRANSACTION_H
