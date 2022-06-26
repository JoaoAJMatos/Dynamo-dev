#include "Miner.h"

#include <utility>

Miner::Miner()
{
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

void Miner::mineBlock()
{
    // Get the valid transactions in the pool
    std::vector<Transaction*> valid_transactions = this->working_transaction_pool->getValidTransactions();

    Blockchain tempChain;

    tempChain.chain = this->working_blockchain->chain;

    // Create a reward transaction
    Block lastBlock = working_blockchain->getLastBlock();
    auto* reward = Transaction::rewardTransaction(lastBlock.getHeight() + 1, rewardAddress);
    valid_transactions.push_back(reward);

    tempChain.addBlock(valid_transactions, 0);
    this->working_blockchain->replaceChain(tempChain);
    this->working_transaction_pool->clear();
    updateStats();
    *sendBlock = 1; // Tell the node to broadcast the block
}

void Miner::start()
{
    while(*mine)
    {
        if (!this->working_transaction_pool->getPool().empty())
        {
            mineBlock();
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

void Miner::setWorkingBlockchain(Blockchain* chain)
{
    this->working_blockchain = chain;
}

void Miner::setWorkingTransactionPool(TransactionPool* pool)
{
    this->working_transaction_pool = pool;
}