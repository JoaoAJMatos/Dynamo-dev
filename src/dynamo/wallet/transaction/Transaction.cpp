//
// Created by Matos on 4/22/2022.
//

#include "Transaction.h"

/* HELPER FUNCTIONS */
/**outputMap* Transaction::createOutput(Wallet* senderWallet, const std::string& recipient, size_t amount)
{
    outputMap tempMap;

    tempMap.amount = amount;
    tempMap.recipient = recipient;
    tempMap.sender = senderWallet->getPublic();
}*/

/**
 * @brief Construct a new Transaction:: Transaction object
 * 
 * @param senderWallet 
 * @param recipient 
 * @param amount 
 */
/**Transaction::Transaction(Wallet* senderWallet, const std::string& recipient, size_t amount)
{
    Dynamo_UUID::uuidv4(this->id); // Creates a UUID for the transaction

    // Create the transaction input and output maps
    this->outMap = createOutput(senderWallet, recipient, amount);
    this->inMap = createInput(senderWallet, this->outMap);
}*/