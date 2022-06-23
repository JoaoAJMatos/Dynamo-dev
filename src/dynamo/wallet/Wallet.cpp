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

Transaction* Wallet::createTransaction(const std::string& recipient, int amount, Blockchain* chain)
{
    // Update the balance
    this->balance = Wallet::calculateBalance(chain, this->keyPair->getPublic());

    // Check if the balance is sufficient
    if (amount > this->balance) return nullptr;

    return new Transaction(this->keyPair, recipient, amount, this->keyPair->getPublic(), this->balance);
}

int Wallet::calculateBalance(Blockchain* chain, const std::string& address)
{
    int outputsTotal = 0;

    for (auto& block : chain->chain)
    {
        for (auto& transaction : block->data)
        {
            if (transaction.getOutputMap().recipient == address) outputsTotal += transaction.getOutputMap().amount;
            if (transaction.getOutputMap().sender == address) outputsTotal -= transaction.getOutputMap().amount;
        }
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