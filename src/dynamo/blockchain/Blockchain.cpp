#include "Blockchain.h"

Blockchain::Blockchain(int isRoot, const std::string& firstNodeAddress)
{
    if (isRoot) this->chain.push_back(Block::genesis(firstNodeAddress, GENESIS_TRANSACTION_REWARD));
    std::cout << "Root chain size: " << this->chain.size() << std::endl;
}

Blockchain::Blockchain(std::string blockchain_packet)
{
    int pos = 0;
    std::string delimiter = "/";

    std::string buffer = blockchain_packet;
    std::string blockString;
    Block* block;

    while ((blockString = buffer.substr(pos, buffer.find("/"))) != "*")
    {
        buffer.erase(0, buffer.find("|-") + delimiter.length() + 2);

        if (blockString != "")
        {
            block = new Block(blockString);
            this->chain.push_back(block);
        }
        else break;
    };
}

Block Blockchain::getLastBlock()
{
    return *this->chain.back();
}

int Blockchain::addBlock(std::vector<Transaction*> data, int log)
{
    auto* newBlock = Block::mineBlock(this->chain.back(), std::move(data), log);

    this->chain.push_back(newBlock);

    return 0;
}

int Blockchain::replaceChain(const Blockchain& _chain)
{
    if (_chain.chain.size() <= this->chain.size())
    {
        std::cout << "[ERROR] The incoming chain is smaller or the same size as the current one" << std::endl;
        std::cout << "Incoming size: " << _chain.chain.size() << " | Your size: " << this->chain.size() << std::endl;
        return 1; // The incoming _chain must be longer
    } 
    if (!Blockchain::isValid(_chain))
    {
        std::cout << "[ERROR] The incoming chain is invalid" << std::endl;
        return 1; // The blocks contained in the _chain must be valid
    }
    if (!Blockchain::isTransactionDataValid(_chain))
    {
        std::cout << "[ERROR] The incoming chain contains invalid transactions" << std::endl;
        return 1; // The transactions contained in the _chain must be valid
    }

    this->chain = _chain.chain;
    return 0;
}

std::string Blockchain::toString(const Blockchain& chain)
{
    std::stringstream ss;
    for (auto block : chain.chain)
    {
        ss << Block::toString(block) << "/";
    }

    ss << "*"; // End of chain

    return ss.str();
}

void Blockchain::printChain() // Print the whole chain
{
    for (auto block : this->chain)
    {
        block->printBlock();
    }
}

void Blockchain::printBlock(int index) // Print a specific block
{
    if (index > this->chain.size())
    {
        std::cout << "[ERROR] The block " << index << " does not exist" << std::endl;
        return;
    }
    this->chain[index]->printBlock();
}

int Blockchain::calculateBalanceOfAddress(const std::string& address)
{
    int outputsTotal = 0;

    for (auto& block : this->chain)
    {
        for (auto& transaction : block->data)
        {
            if (transaction->getOutputMap().recipient == address) outputsTotal += transaction->getOutputMap().amount;
            if (transaction->getOutputMap().sender == address) outputsTotal -= transaction->getOutputMap().amount;
        }
    }
    return outputsTotal; 
}

int Blockchain::isTransactionDataValid(Blockchain blockchain)
{
    for (auto& block : blockchain.chain)
    {
        int rewardTransactionCount = 0;
        std::map<char*, Transaction*> transactions; // This will be used to ensure that there are no duplicate transactions

        for (auto& transaction : block->data)
        {
            if (transaction->getInputMap().address == "dynamo-consensus-node-reward")
            {
                rewardTransactionCount++;

                if (rewardTransactionCount > 1)
                {
                    std::cout << "[ERROR] There are more than one reward transactions in block" << std::endl;
                    return 0; // Block has more than one reward transaction
                } 
                if (transaction->getOutputMap().amount != Transaction::getBlockSubsidy(block->getHeight()))
                {
                    std::cout << "[ERROR] The reward transaction amount is not correct" << std::endl;
                    return 0; // Mining reward isn't correct
                } 
            }
            else 
            {
                if(!Transaction::validTransaction(transaction)) return 0; // Transaction is not valid

                int trueBalance = calculateBalanceOfAddress(transaction->getInputMap().address);

                if (((strcmp(transaction->getInputMap().address.c_str(), "Genesis") != 0) && (strcmp(transaction->getInputMap().address.c_str(), "dynamo-consensus-node-reward") != 0)) && (transaction->getInputMap().balance != trueBalance))
                {
                    std::cout << "[ERROR] Blockchain Validation Error: On block " << block->getHeight() << " the balance of " << transaction->getInputMap().address << " (" << transaction->getInputMap().balance << ") does not match the expected balance of: " << trueBalance << std::endl;
                    return 0; // Sender's balance is not accurate
                }

                if (!transactions.insert(std::pair<char *, Transaction*>(transaction->getID(), transaction)).second)
                {
                    std::cout << "[ERROR] Blockchain Validation Error: On block " << block->getHeight() << " the transaction " << transaction->getID() << " is a duplicate" << std::endl;
                    return 0; // Transaction is a duplicate
                }
            }
        }
    }

    return 1;
}

int Blockchain::isValid(Blockchain chain)
{
    SHA256 sha;
    std::stringstream blockBuffer;

    // Check if the genesis block of the chain matches the agreed genesis block
    if (chain.chain[0]->getHeight() != 0 || chain.chain[0]->data[0]->getInputMap().address != "Genesis") return 0;

    for (int i = 1; i < chain.chain.size(); i++)
    {
        Block* block = chain.chain[i];
        Block* lastBlock = chain.chain[i-1];

        if (block->getHeight() != i) return 0; // Check if the height is correct
        if (block->getPrevHash() != lastBlock->getHash()) return 0; // Check if the previous hash is correct
        
        // Check if the hash is correct
        std::string dataToHash;
        for (auto& transaction : block->data)
        {
            dataToHash.append(transaction->getTransactionDataBuffer());
        }

        blockBuffer << block->getTimestamp() << block->getHeight() << block->getPrevHash() << dataToHash << block->getDifficulty() << block->getNonce() << std::endl;
    
        sha.update(blockBuffer.str());
        uint8_t* hash = sha.digest();

        if (block->getHash() != hash) return 0;

        if (lastBlock->getDifficulty() - block->getDifficulty() > DIFFICULTY_ADJUSTMENT_INTERVAL) return 0; // Prevent difficulty jumps
    }

    return 1;
}