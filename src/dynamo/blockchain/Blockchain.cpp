#include "Blockchain.h"

Blockchain::Blockchain(int isRoot, const std::string& firstNodeAddress)
{
    if (isRoot) this->chain.push_back(Block::genesis(firstNodeAddress, GENESIS_TRANSACTION_REWARD));
    std::cout << "Root chain size: " << this->chain.size() << std::endl;
}

Blockchain::Blockchain(std::string blockchain_packet)
{
    using namespace msgpack11;

    // Deserialize the blockchain packet
    std::string err;
    MsgPack blockchain = MsgPack::parse(blockchain_packet, err);

    std::string addr = "hehe";
    Blockchain tempChain(0, addr);

    try
    {
        std::cout << "Chain array size: " << blockchain["chain"].array_items().size() << std::endl;

        for (auto& block : blockchain["chain"].array_items())
        {
            Block* newBlock = new Block(block.dump());
            tempChain.chain.push_back(newBlock);
        }

        int res = this->replaceChain(tempChain);
        std::cout << "Replace chain result: " << res << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cout << "[ERROR] Unable to create a blockchain instance from the incomming packet." << std::endl;
    }
}

int Blockchain::addBlock(std::vector<Transaction> data, int log)
{
    Block* newBlock = Block::mineBlock(this->chain.back(), data, log);

    this->chain.push_back(newBlock);

    return 0;
}

int Blockchain::replaceChain(Blockchain chain)
{
    if (chain.chain.size() <= this->chain.size())
    {
        std::cout << "[ERROR] The incomming chain is smaller than the current one" << std::endl;
        std::cout << "Incomming size: " << chain.chain.size() << " | Your size: " << this->chain.size() << std::endl; 
        return 1; // The incomming chain must be longer
    } 
    if (!Blockchain::isValid(chain))
    {
        std::cout << "[ERROR] The incomming chain is invalid" << std::endl;
        return 1; // The blocks contained in the chain must be valid
    }
    if (!Blockchain::isTransactionDataValid(chain))
    {
        std::cout << "[ERROR] The incomming chain contains invalid transactions" << std::endl;
        return 1; // The transactions contained in the chain must be valid
    }

    this->chain = chain.chain;
    return 0;
}

msgpack11::MsgPack Blockchain::serialize(Blockchain chain)
{
    using namespace msgpack11;

    MsgPack tempChain;
    MsgPack::array tempChainArray;

    for (auto& block : chain.chain)
    {
        tempChainArray.push_back(Block::serialize(block));
    }

    tempChain = tempChainArray;

    MsgPack tempBlockchain = MsgPack::object {
        {"chain", tempChain}
    };

    return tempBlockchain;
}

void Blockchain::printChain() // Print the whole chain
{
    for (auto& block : this->chain)
    {
        block->printBlock();
    }
}

void Blockchain::printBlock(int index) // Print a specific block
{
    this->chain[index]->printBlock();
}

int Blockchain::calculateBalanceOfAddress(const std::string& address)
{
    int outputsTotal = 0;

    for (auto& block : this->chain)
    {
        for (auto& transaction : block->data)
        {
            if (transaction.getOutputMap().recipient == address) outputsTotal += transaction.getOutputMap().amount;
            if (transaction.getOutputMap().sender == address) outputsTotal -= transaction.getOutputMap().amount;
        }
    }
    return outputsTotal; 
}

int Blockchain::isTransactionDataValid(Blockchain chain)
{
    for (auto& block : chain.chain)
    {
        int rewardTransactionCount = 0;
        std::map<char*, Transaction> transactions; // This will be used to ensure that there are no duplicate transactions

        for (auto& transaction : block->data)
        {
            if (transaction.getInputMap().address == "dynamo-consensus-node-reward")
            {
                rewardTransactionCount++;

                if (rewardTransactionCount > 1) return 0; // Block has more than one reward transaction
                if (transaction.getOutputMap().amount != Transaction::getBlockSubsidy(block->getHeight())) return 0; // Mining reward isn't correct
            }
            else 
            {
                if(Transaction::validTransaction(&transaction)) return 0; // Transaction is not valid

                int trueBalance = calculateBalanceOfAddress(transaction.getInputMap().address);

                if (transaction.getInputMap().balance != trueBalance) return 0; // Sender's balance is not accurate

                if (transactions.insert(std::pair<char*, Transaction>(transaction.getID(), transaction)).second == false) return 0; // Transaction is a duplicate
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
    if (chain.chain[0]->getHeight() != 0 || chain.chain[0]->data[0].getInputMap().address != "Genesis") return 0;

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
            dataToHash.append(transaction.getTransactionDataBuffer());
        }

        blockBuffer << block->getTimestamp() << block->getHeight() << block->getPrevHash() << dataToHash << block->getDifficulty() << block->getNonce() << std::endl;
    
        sha.update(blockBuffer.str());
        uint8_t* hash = sha.digest();

        if (block->getHash() != hash) return 0;

        if (lastBlock->getDifficulty() - block->getDifficulty() > 1) return 0; // Prevent difficulty jumps
    }

    return 1;
}