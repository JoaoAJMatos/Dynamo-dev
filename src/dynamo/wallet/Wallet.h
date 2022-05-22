//
// Created by Matos on 4/22/2022.
//

#ifndef DEV_DYNAMO_WALLET_H
#define DEV_DYNAMO_WALLET_H

#include "../../crypto/EC/ECDSA.h"
#include "../blockchain/Blockchain.h"
//#include "transaction/Transaction.h"

template <class T>
class Wallet;

/** 
 * @brief The output map contains information about the sender and the recipient 
 * 
 */
struct outputMap
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
    
    // The outputMap and inputMap structs
    outputMap outMap;
    inputMap inMap;


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
    outputMap* createOutput(Wallet<int>* senderWallet, const std::string& recipient, size_t amount);

    /**
     * @brief Create a Input object
     * 
     * @param senderWallet 
     * @param outputMap 
     * @return inputMap* 
     *
     * @details This helper function creates an input map for the transaction
     */
    inputMap* createInput(Wallet<int>* senderWallet, outputMap* outputMap);


public:
    /* CONSTRUCTOR */
    /**
     * @brief Construct a new Transaction object
     * 
     * @param senderWallet 
     * @param recipient 
     * @param amount 
     */
    Transaction(Wallet<int>* senderWallet, const std::string& recipient, size_t amount);

    /* PUBLIC FUNCTIONS */
    /**
     * @brief Updates transactions and resigns them
     * 
     * @return int 
     */
    int update(Wallet<int>* senderWallet, const std::string& recipient, size_t amount);

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
     */
    static int validTransaction(Transaction* transaction);
};

template <class T>
class Wallet
{
private:
    /* MEMBER VARIABLES */

    // A pointer to a ECDSA class instance for signing and verifying transactions
    ECDSA* keyPair;

    unsigned long long int balance;

    // A pointer to a blockchain. This is needed in order to calculate the balance for the wallet
    Blockchain* chain;

public:
    /* CONSTRUCTOR */
    Wallet(Blockchain* chain, const char* address, const char* privateKey);

    /* PUBLIC FUNCTIONS */
    /**
     * @brief Signs a string buffer using the wallet's private key
     * 
     * @param data 
     * @return int 
     */
    int sign(const std::string& data);

    /**
     * @brief Create a Transaction object
     * 
     * @param recipient 
     * @param amount 
     * @param chain 
     * @return std::string
     * 
     * @details This function should return a buffer containing the transaction information 
     */
    Transaction* createTransaction(const std::string& recipient, int amount, Blockchain chain);

    /* STATIC FUNCTIONS */
    /**
     * @brief Calculates the balance for a given address in the blockchain
     * 
     * @param chain 
     * @param address 
     * @return int 
     */
    static int calculateBalance(Blockchain chain, const std::string& address);
};


#endif //DEV_DYNAMO_WALLET_H
