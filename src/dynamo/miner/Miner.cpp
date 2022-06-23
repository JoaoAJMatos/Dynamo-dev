#include "Miner.h"

#include <utility>

Miner::Miner(Blockchain* working_blockchain, TransactionPool* working_transaction_pool)
{
    this->working_blockchain = working_blockchain;
    this->working_transaction_pool = working_transaction_pool;
    this->stats = new Stats;

    this->stats->blocks_mined = 0;
    this->stats->rewards = 0;
}

void Miner::setRewardAddress(std::string rewardAddr)
{
    this->rewardAddress = std::move(rewardAddr);
}

void Miner::setLog(int* log_flag)
{
    this->log = log_flag;
}

void Miner::setMine(bool* mine_flag)
{
    this->mine = mine_flag;
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

        if(!valid_transactions.empty())
        {
            // Create a reward trasnaction
            Block lastBlock = working_blockchain->getLastBlock();
            Transaction reward = Transaction::rewardTransaction(lastBlock.getHeight() + 1, rewardAddress);
            valid_transactions.push_back(reward);

            this->working_blockchain->addBlock(valid_transactions, *log);
            this->working_transaction_pool->clear();
            updateStats();
            *this->sendBlock = 1;
        }
    }
}

Stats* Miner::getStats()
{
    return this->stats;
}

void Miner::setClient(NodeClient* pclient)
{
    this->client = pclient;
}

void Miner::setKnownHosts(hostMap* pKnown_hosts)
{
    this->known_hosts = pKnown_hosts;
}

void Miner::setSendBlock(int* sendBlock)
{
    this->sendBlock = sendBlock;
}