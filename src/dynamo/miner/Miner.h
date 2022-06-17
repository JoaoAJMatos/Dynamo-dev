#ifndef MINER_H
#define MINER_H

#include "../blockchain/Blockchain.h"
#include "../wallet/transaction/TransactionPool.h"

class Miner
{
private:
    Blockchain* working_blockchain;
    TransactionPool* working_transaction_pool;

public:
    Miner(Blockchain* working_blockchain, TransactionPool* working_transaction_pool);
    ~Miner();

    void start();
};

#endif