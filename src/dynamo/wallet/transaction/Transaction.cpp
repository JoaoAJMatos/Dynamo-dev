//
// Created by Matos on 4/22/2022.
//

#include "Transaction.h"

Transaction::Transaction(ECDSA* keyPair, const std::string& recipient, size_t amount, const std::string& sender, size_t balanceBeforeTransaction)
{
    Dynamo_UUID::uuidv4(this->id); // Create a UUID for the transaction

    createOutput(recipient, amount, sender, balanceBeforeTransaction);
    createInput(keyPair, &this->outMap);
}

/**
 * @brief Creates an output map for the transaction
 * 
 * @param recipient 
 * @param amount 
 * @param sender 
 * @param balanceBeforeTransaction 
 * @return int 
 */
int Transaction::createOutput(const std::string& recipient, size_t amount, const std::string& sender, size_t balanceBeforeTransaction)
{
    this->outMap.recipient = recipient;
    this->outMap.amount = amount;
    this->outMap.sender = sender;
    this->outMap.balance = balanceBeforeTransaction - amount;

    return 0;
}

/**
 * @brief Takes in the output map and creates an input map for the transaction
 * 
 * @param keyPair 
 * @param outputMap 
 * @return int 
 */
int Transaction::createInput(ECDSA* keyPair, outputMap* outputMap)
{
    this->inMap.timestamp = Time::getTimestamp();
    this->inMap.balance = outputMap->balance + outputMap->amount;
    this->inMap.address = outputMap->sender;

    // Hash the output map and sign it with the sender's private key
    // Create the string buffer with the output map data
    std::string buffer = this->outMap.recipient 
                         + std::to_string(this->outMap.amount) 
                         + this->outMap.sender 
                         + std::to_string(this->outMap.balance);

    sha256.update(buffer);
    
    uint8_t* hash = sha256.digest();

    outputMapHash = SHA256::toString(hash);

    keyPair->signHash(outputMapHash.c_str());

    this->inMap.signature = keyPair->getSignatureHex();

    return 0;
}


/**
 * @brief Validates a transaction
 * 
 * @param transaction 
 * @param keyPair 
 * @return int 
 */
int Transaction::validTransaction(Transaction* transaction, ECDSA* keyPair)
{
    // Check if the signature is valid
    if(!(keyPair->verifySignature(transaction->inMap.address.c_str(), transaction->outputMapHash.c_str(), transaction->inMap.signature.c_str()))) return 0;

    // Check if the balance is sufficient
    if(transaction->inMap.balance < transaction->outMap.amount) return 0;

    return 0;
}

/**
 * @brief 
 * 
 * @return std::string 
 */
std::string Transaction::getTransactionDataBuffer()
{
    // Create the string buffer with the output map data
    std::string buffer = this->outMap.recipient 
                         + std::to_string(this->outMap.amount) 
                         + this->outMap.sender 
                         + std::to_string(this->outMap.balance)
                         + std::to_string(this->inMap.timestamp)
                         + std::to_string(this->inMap.balance)
                         + this->inMap.address
                         + this->inMap.signature;

    return buffer;
}

/**
 * @brief Prints the transaction contents to the standard output
 * 
 */
void Transaction::showTransaction()
{
    std::cout << "      ID: " << this->id << std::endl;
    std::cout << "      {" << std::endl;
    std::cout << "          OutputMap: " << std::endl;
    std::cout << "          {" << std::endl;
    std::cout << "              Recipient: " << this->outMap.recipient << std::endl;
    std::cout << "              Amount: " << this->outMap.amount << std::endl;
    std::cout << "              Sender: " << this->outMap.sender << std::endl;
    std::cout << "              Balance: " << this->outMap.balance << std::endl;
    std::cout << "          }," << std::endl;
    std::cout << "          Input Map: " << std::endl;
    std::cout << "          {" << std::endl;
    std::cout << "              Timestamp: " << this->inMap.timestamp << std::endl;
    std::cout << "              Balance: " << this->inMap.balance << std::endl;
    std::cout << "              Address: " << this->inMap.address << std::endl;
    std::cout << "              Signature: " << this->inMap.signature << std::endl;
    std::cout << "          }" << std::endl;
    std::cout << "      }" << std::endl;
}