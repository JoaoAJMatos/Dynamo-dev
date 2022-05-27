#include "Blockchain.h"

Blockchain::Blockchain(int isRoot, const std::string& firstNodeAddress)
{
    if (isRoot) this->chain.push_back(Block::genesis(firstNodeAddress, GENESIS_TRANSACTION_REWARD));
}

int Blockchain::addBlock(std::vector<Transaction> data, int log)
{
    return 0;
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

std::vector<Block*> Blockchain::getChain()
{
    return this->chain;
}