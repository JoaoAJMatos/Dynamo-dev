//
// Created by Matos on 4/22/2022.
//

#ifndef DEV_DYNAMO_TRANSACTION_H
#define DEV_DYNAMO_TRANSACTION_H

#include <string>

#include "../../../../src/util/uuid/uuid.h"
#include "../../../system/time/Time.h"
#include "../../../crypto/EC/ECDSA.h"
#include "../../../crypto/SHA-2/SHA256.h"

/** 
 * @brief The output map contains information about the sender and the recipient 
 * 
 */
struct outputMap
{
    std::string recipient;    // The recipients address
    size_t amount;            // The amount to be transacted
    std::string sender;       // The senders address
    size_t balance;           // The senders balance after the transaction
};

/**
 * @brief The input map contains critical information for the validation of the transaction
 *        like the timestamp of when the transaction was emmited, the balance the sender had before
 *        making the transaction, the public address of the sender and most importantly the transaction's
 *        digital signature.
 * 
 *        The transaction signature is obtained by signing a hash of the output map with the sender's private key
 */
struct inputMap
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
    std::string outputMapHash;
    
    // The outputMap and inputMap structs
    outputMap outMap;
    inputMap inMap;

    SHA256 sha256;

    /* PRIVATE FUNCTIONS */
    /**
     * @brief Create a Output object
     * 
     * @param senderWallet 
     * @param recipient 
     * @param amount 
     * @return outputMap* 
     * 
     * @details This helper function creates an Output Map for the transaction
     */
    int createOutput(const std::string& recipient, size_t amount, const std::string& sender, size_t balanceBeforeTransaction);

    /**
     * @brief Create a Input object
     * 
     * @param senderWallet 
     * @param outputMap 
     * @return inputMap* 
     *
     * @details This helper function creates an input map for the transaction
     */
    int createInput(ECDSA* keyPair, outputMap* outputMap);


public:
    /* CONSTRUCTOR */
    /**
     * @brief Construct a new Transaction object
     * 
     * @param senderWallet 
     * @param recipient 
     * @param amount 
     */
    Transaction(ECDSA* keyPair, const std::string& recipient, size_t amount, const std::string& sender, size_t balanceBeforeTransaction);

    /* PUBLIC FUNCTIONS */
    /**
     * @brief Updates transactions and resigns them
     * 
     * @return int 
     */
    int update(ECDSA* keyPair, const std::string& recipient, size_t amount);

    /**
     * @brief Returns a string buffer containing the transaction data to pass into the hashing function to create the block
     * 
     * @return std::string 
     */
    std::string getTransactionDataBuffer();

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
    static Transaction* rewardTransaction();

    /**
     * @brief Validates a transaction
     * 
     * @param transaction 
     * @return int 
     * 
     * @details This function validates a transaction by checking the digital signature of the input map. 
     *          Returns 1 if the transaction is valid and 0 if it is not.
     */
    static int validTransaction(Transaction* transaction, ECDSA* keyPair);
};

#endif //DEV_DYNAMO_TRANSACTION_H
