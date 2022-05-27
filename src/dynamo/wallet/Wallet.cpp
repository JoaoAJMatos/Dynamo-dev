//
// Created by Matos on 4/22/2022.
//

#include "Wallet.h"

Wallet::Wallet() 
{
    this->keyPair = new ECDSA;
    this->balance = 0;
}

Wallet::Wallet(char* address, char* privateKey)
{
    this->keyPair = new ECDSA(address, privateKey);
    this->balance = 0;
}

Transaction* Wallet::createTransaction(const std::string& recipient, int amount, Blockchain chain)
{
    // Update the balance
    this->balance = Wallet::calculateBalance(chain, this->keyPair->getPublic());

    // Check if the balance is sufficient
    if (amount > this->balance)
    {
        std::cout << "[ERR] Transaction - Insufficient funds: " << this->balance << std::endl;
        return nullptr;
    }

    return new Transaction(this->keyPair, recipient, amount, this->keyPair->getPublic(), this->balance);
}

int Wallet::calculateBalance(Blockchain chain, const std::string& address)
{
    bool hasConductedTransaction = false;
    int outputsTotal = 0;

    for (auto& block : chain.getChain())
    {
        for (auto& transaction : block->getData())
        {
            // Check if the transaction has been conducted by the wallet
            if (transaction.getInputMap().address == address) hasConductedTransaction = true;

            outputsTotal += transaction.getOutputMap().balance;
        }
    
        if (hasConductedTransaction) break;
    }

    return outputsTotal;
}

char* Wallet::getAddress()
{
    return this->keyPair->getPublicKeyHex();
}

char* Wallet::getPrivate()
{
    return this->keyPair->getPrivateKeyHex();
}