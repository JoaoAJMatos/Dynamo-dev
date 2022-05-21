//
// Created by Matos on 4/22/2022.
//

#ifndef DEV_DYNAMO_WALLET_H
#define DEV_DYNAMO_WALLET_H

#include "../../crypto/EC/ECDSA.h"
#include "../blockchain/Blockchain.h"
#include "transaction/Transaction.h"

class Wallet
{
private:
    /* MEMBER VARIABLES */

    // A pointer to a ECDSA class instance for signing and verifying transactions
    ECDSA* keyPair;

    unsigned long long int balance;

public:
    /* CONSTRUCTOR */
    Wallet();

    /* PUBLIC FUNCTIONS */
    int sign();

    /**
     * @brief Create a Transaction object
     * 
     * @param recipient 
     * @param amount 
     * @param chain 
     * @return Transaction 
     */
    Transaction createTransaction(const std::string& recipient, int amount, Blockchain chain);

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
