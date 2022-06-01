#ifndef DEV_DYNAMO_TRANSACTION_POOL_H
#define DEV_DYNAMO_TRANSACTION_POOL_H

#include <vector>
#include <map>

#include "Transaction.h"

class TransactionPool
{
private:
    std::vector<Transaction> pool;

public:
    TransactionPool();

    /**
     * @brief Clear the transaction pool
     * 
     */
    void clear();

    int setTransaction(Transaction transaction);

    /**
     * @brief Set this transaction pool to an incomming one
     * 
     * @param pool 
     * @return int 
     */
    int set(std::vector<Transaction> pool);

    /**
     * @brief Checks if there's a transaction with the same sender
     * 
     * @param sender 
     * @return int 
     */
    int existsTransaction(const std::string& sender);

    /**
     * @brief Adds transactions to the valid transactions array
     * 
     * @param transaction 
     */
    std::vector<Transaction> getValidTransactions();

    /**
     * @brief Add a transaction to the pool
     * 
     * @param transaction 
     */
    void addTransaction(Transaction transaction);
};

#endif 