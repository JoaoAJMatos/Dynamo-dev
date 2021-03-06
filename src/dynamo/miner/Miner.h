#ifndef MINER_H
#define MINER_H

#include "../blockchain/Blockchain.h"
#include "../wallet/transaction/TransactionPool.h"
#include "../network/node/common/NodeClient.h"
#include "../network/protocol/DTP.h"

typedef std::vector<std::pair<std::string, int>> hostMap;

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
    
    NodeClient* client;
    hostMap* known_hosts;

    bool* mine;  // Indicate if the miner should mine or not 
    int* log;    // Indicate if the miner should log or not
    int* sendBlock; // Indicate if the miner should send a block or not

    std::string rewardAddress; // The destination address of the reward transaction
    Stats* stats;              // The stats of the miner

    void updateStats();

public:
    Miner();

    void start();
    void setMine(bool* mine_flag);
    void setRewardAddress(std::string rewardAddr);
    void setLog(int* log_flag);
    void setClient(NodeClient* pClient);
    void setKnownHosts(hostMap* known_hosts);
    void setSendBlock(int* sendBlock);
    void setWorkingBlockchain(Blockchain* chain);
    void setWorkingTransactionPool(TransactionPool* pool);

    void mineBlock();

    void broadCastChain();

    Stats* getStats();
};

#endif