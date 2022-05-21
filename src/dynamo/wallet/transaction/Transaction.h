//
// Created by Matos on 4/22/2022.
//

#ifndef DEV_DYNAMO_TRANSACTION_H
#define DEV_DYNAMO_TRANSACTION_H

#include <string>

#include "../../../../src/util/uuid/uuid.h"

/** 
 * @brief The output map contains information about the sender and the recipient 
 * 
 */
typedef struct outputMap
{
    std::string recipient; // The recipients address
    int amount;            // The amount to be transacted
    std::string sender;    // The senders address
    int balance;           // The senders balance after the transaction
};

/**
 * @brief The input map contains critical information for the validation of the transaction
 *        like the timestamp of when the transaction was emmited, the balance the sender had before
 *        making the transaction, the public address of the sender and most importantly the transaction's
 *        digital signature.
 * 
 *        The transaction signature is obtained by signing a hash of the output map with the sender's private key
 */
typedef struct inputMap
{
    time_t timestamp;      // The time the transaction was created
    int balance;           // The balance of the sender before the transaction
    std::string address;   // The address of the sender
    std::string signature; // The digital signature of the transaction
};

class Transaction
{
private:
    /* MEMBER VARIABLES */
    // The UUID of the transaction
    char id[38];
    
    // The outputMap and inputMap structs
    outputMap outMap;
    inputMap inMap;


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

    /**
     * @brief Prints the transaction data into the standard-out 
     * 
     */
    void showTransaction();

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
