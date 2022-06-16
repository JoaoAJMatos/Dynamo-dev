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

Transaction::Transaction(std::string transaction_packet)
{
    /*using namespace msgpack11;

    // Deserialize the transaction packet
    std::string err;
    MsgPack transaction = MsgPack::parse(transaction_packet, err);

    // Create a transaction object
    try
    {
        strcpy(this->id, transaction["id"].string_value().c_str());
        createOutput(transaction["outMap"]["recipient"].string_value(), (size_t)transaction["outMap"]["amount"].int_value(), transaction["outMap"]["sender"].string_value(), (size_t)transaction["inMap"]["balance"].int_value());

        this->inMap.timestamp = transaction["inMap"]["timestamp"].int_value();
        this->inMap.balance = transaction["inMap"]["balance"].int_value();
        this->inMap.address = transaction["inMap"]["address"].string_value();
        this->inMap.signature = transaction["inMap"]["signature"].string_value();
    }
    catch(const std::exception& e)
    {
        std::cout << "[ERROR] Unable to create a transaction instance from the incomming packet." << std::endl;
    }*/

    int pos = 0;
    std::string delimiter = ";";

    std::string buffer = transaction_packet;

    strcpy(this->id, buffer.substr(pos, buffer.find(delimiter)).c_str());
    buffer.erase(0, buffer.find(delimiter) + delimiter.length());

    std::string recipient = buffer.substr(pos, buffer.find(delimiter));
    buffer.erase(0, buffer.find(delimiter) + delimiter.length());

    int amount = atoi(buffer.substr(pos, buffer.find(delimiter)).c_str());
    buffer.erase(0, buffer.find(delimiter) + delimiter.length());

    std::string sender = buffer.substr(pos, buffer.find(delimiter));
    buffer.erase(0, buffer.find(delimiter) + delimiter.length());

    int balanceAfterTransaction = atoi(buffer.substr(pos, buffer.find(delimiter)).c_str());
    buffer.erase(0, buffer.find(delimiter) + delimiter.length());

    this->inMap.timestamp = (time_t)atoi(buffer.substr(pos, buffer.find(delimiter)).c_str());
    buffer.erase(0, buffer.find(delimiter) + delimiter.length());

    this->inMap.balance = (size_t)atoi(buffer.substr(pos, buffer.find(delimiter)).c_str());
    buffer.erase(0, buffer.find(delimiter) + delimiter.length());

    this->inMap.address = buffer.substr(pos, buffer.find(delimiter));
    buffer.erase(0, buffer.find(delimiter) + delimiter.length());

    this->inMap.signature = buffer.substr(pos, buffer.find(delimiter));

    createOutput(recipient, (size_t)amount, sender, (size_t)this->inMap.balance);
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

    if (keyPair)
    {
        // Hash the output map and sign it with the sender's private key
        // Create the string buffer with the output map data
        std::string buffer = this->outMap.recipient 
                            + std::to_string(this->outMap.amount) 
                            + this->outMap.sender 
                            + std::to_string(this->outMap.balance);

        sha256.update(buffer);
        
        uint8_t* hash = sha256.digest();

        outputMapHash = SHA256::toString(hash);

        int res = keyPair->signHash(outputMapHash.c_str());

        this->inMap.signature = keyPair->getSignatureHex();

        return res;
    }

    this->inMap.signature = "Dynamo-Authorized-Signature";

    return 0;
}


/**
 * @brief Validates a transaction
 * 
 * @param transaction 
 * @param keyPair 
 * @return int 
 */
int Transaction::validTransaction(Transaction* transaction)
{
    ECDSA keyPair;

    std::cout << "Sender: " << transaction->inMap.address << std::endl;
    std::cout << "strcmp out: " << strcmp(transaction->inMap.address.c_str(), "Genesis") << std::endl;

    // Check if the transaction is comming from the Genesis block or is a reward transaction
    if ((strcmp(transaction->inMap.address.c_str(), "Genesis") == 0) || (strcmp(transaction->inMap.address.c_str(), "dynamo-consensus-node-reward") == 0))
    {
        // Check if the balance is sufficient
        if(transaction->inMap.balance >= transaction->outMap.amount) return 1;
    }
    
    // Check if the signature is valid
    if(keyPair.verifySignature(transaction->inMap.address.c_str(), transaction->outputMapHash.c_str(), transaction->inMap.signature.c_str()))
    {
        // Check if the balance is sufficient
        if(transaction->inMap.balance >= transaction->outMap.amount) return 1;
    } 

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
* @brief Get the Block Subsidy for the mined block
* 
* @param height 
* @param concensus_halving_interval 
* @return int 
* 
* @details This function calculates the block subsidy based on the block height and the consensus halving interval
*/
int Transaction::getBlockSubsidy(int height)
{
    int halvings = height / CONSENSUS_HALVING_INTERVAL; // The amount of halvings that have taken place

    if (halvings >= 64) return 0; // Force block reward to 0 when the right-shift is undefined, meaning no more halvings can take place

    int subsidy = INITIAL_SUBSIDY * COIN; // Initial block subsidy in BLOX

    // Subsidy will cut in half every 210,000 blocks
    subsidy >>= halvings;
    return subsidy;
}

/**
 * @brief Updates transactions and resigns them
 * 
 * @param keyPair 
 * @param recipient 
 * @param amount 
 * @return int 
 */
int Transaction::update(ECDSA* keyPair, size_t amount)
{
    if (amount > this->outMap.balance) return 1;

    if (amount > 0)
    {
        this->outMap.amount += amount;
        this->outMap.balance -= amount;
    }
    else
    {
        this->outMap.amount -= amount;
        this->outMap.balance += abs((int)amount);
    }

    // Resign the transaction
    this->createInput(keyPair, &this->outMap);

    return 0;
}

/**
 * @brief Creates a reward transaction
 * 
 * @return Transaction* 
 */
Transaction Transaction::rewardTransaction(size_t blockHeight, const std::string& recipient)
{
    int reward = getBlockSubsidy(blockHeight);

    return Transaction(nullptr, recipient, reward, "dynamo-consensus-node-reward", reward);
}

msgpack11::MsgPack Transaction::serialize(Transaction* transaction)
{
    using namespace msgpack11;

    MsgPack transaction_pack = MsgPack::object {
        {"id", transaction->getID()},
        {"outMap", MsgPack::object {
            {"recipient", transaction->getOutputMap().recipient},
            {"amount", transaction->getOutputMap().amount},
            {"sender", transaction->getOutputMap().sender},
            {"balance", transaction->getOutputMap().balance}
        }},
        {"inMap", MsgPack::object {
            {"timestamp", transaction->getInputMap().timestamp},
            {"balance", transaction->getInputMap().balance},
            {"address", transaction->getInputMap().address},
            {"signature", transaction->getInputMap().signature}
        }}
    };

    return transaction_pack;
}

std::string Transaction::toString(Transaction* transaction)
{
    std::stringstream ss;
    ss << transaction->getID() << ";" << transaction->getOutputMap().recipient << ";" << transaction->getOutputMap().amount << ";" << transaction->getOutputMap().sender << ";" << transaction->getOutputMap().balance << ";" << transaction->getInputMap().timestamp << ";" << transaction->getInputMap().balance << ";" << transaction->getInputMap().address << ";" << transaction->getInputMap().signature << ";";
    return ss.str();
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

/* GETTERS */
inputMap Transaction::getInputMap()
{
    return this->inMap;
}

outputMap Transaction::getOutputMap()
{
    return this->outMap;
}

char* Transaction::getID()
{
    return this->id;
}