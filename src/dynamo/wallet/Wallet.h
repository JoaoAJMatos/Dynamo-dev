//
// Created by Matos on 4/22/2022.
//

#ifndef DEV_DYNAMO_WALLET_H
#define DEV_DYNAMO_WALLET_H

#include "../../crypto/EC/ECDSA.h"
#include "../blockchain/Blockchain.h"
#include "../../crypto/SHA-2/SHA256.h"
#include "transaction/Transaction.h"

class Wallet
{
private:
    /* MEMBER VARIABLES */
    // A pointer to a ECDSA class instance for signing and verifying transactions
    ECDSA* keyPair;

    SHA256 sha;

    unsigned long long int balance;

public:
    /* CONSTRUCTOR */
    Wallet(char* address, char* privateKey);
    Wallet();

    /* PUBLIC FUNCTIONS */
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
    Transaction* createTransaction(const std::string& recipient, int amount, Blockchain* chain);

    /* STATIC FUNCTIONS */
    /**
     * @brief Calculates the balance for a given address in the blockchain
     * 
     * @param chain 
     * @param address 
     * @return int 
     */
    static int calculateBalance(Blockchain* chain, const std::string& address);

    /* GETTERS */
    char* getAddress();
    char* getPrivate();
};


#endif //DEV_DYNAMO_WALLET_H
