#ifndef MINER_H
#define MINER_H

#include "../blockchain/Blockchain.h"
#include "../wallet/transaction/TransactionPool.h"

struct Stats
{
    int blocks_mined;// The number of blocks mined
    int rewards;// The number of rewards received
};

class Miner
{
private:
    Blockchain* working_blockchain;
    TransactionPool* working_transaction_pool;
    
    bool* mine;  // Indicate if the miner should mine or not 
    int* log;    // Indicate if the miner should log or not

    std::string rewardAddress; // The destination address of the reward transaction
    Stats* stats;              // The stats of the miner

    void updateStats();

public:
    Miner(Blockchain* working_blockchain, TransactionPool* working_transaction_pool);

    void start();
    void setMine(bool* mine_flag);
    void setRewardAddress(std::string rewardAddr);
    void setLog(int* log_flag);

    Stats* getStats();
};

#endif