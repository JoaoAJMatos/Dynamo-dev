#include "Miner.h"

Miner::Miner(Blockchain* working_blockchain, TransactionPool* working_transaction_pool)
{
    this->working_blockchain = working_blockchain;
    this->working_transaction_pool = working_transaction_pool;
    this->stats = new Stats;

    this->stats->blocks_mined = 0;
    this->stats->rewards = 0;
}

void Miner::setRewardAddress(std::string rewardAddress)
{
    this->rewardAddress = rewardAddress;
}

void Miner::setLog(int* log)
{
    this->log = log;
}

void Miner::setMine(bool* mine)
{
    this->mine = mine;
}

void Miner::updateStats()
{
    this->stats->blocks_mined++;
    this->stats->rewards += Transaction::getBlockSubsidy(this->working_blockchain->getLastBlock().getHeight());
}

void Miner::start()
{
    while(*mine)
    {
        // Get the valid transactions in the pool
        std::vector<Transaction> valid_transactions = working_transaction_pool->getValidTransactions();

        if(valid_transactions.size() > 0)
        {
            // Create a reward trasnaction
            Block lastBlock = working_blockchain->getLastBlock();
            Transaction reward = Transaction::rewardTransaction(lastBlock.getHeight() + 1, rewardAddress);
            valid_transactions.push_back(reward);

            this->working_blockchain->addBlock(valid_transactions, *log);
            this->working_transaction_pool->clear();
            updateStats();
        }
    }
}

Stats* Miner::getStats()
{
    return this->stats;
}