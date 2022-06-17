#include "Miner.h"

Miner::Miner(Blockchain* working_blockchain, TransactionPool* working_transaction_pool)
{
    this->working_blockchain = working_blockchain;
    this->working_transaction_pool = working_transaction_pool;
}