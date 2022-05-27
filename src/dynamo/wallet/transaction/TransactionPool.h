#ifndef DEV_DYNAMO_TRANSACTION_POOL_H
#define DEV_DYNAMO_TRANSACTION_POOL_H

#include <vector>
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

    /**
     * @brief Set this transaction pool to an incomming one
     * 
     * @param pool 
     * @return int 
     */
    int set(std::vector<Transaction> pool);

    /**
     * @brief Checks if there's a transaction with the same sender and recipient
     * 
     * @param sender 
     * @return int 
     */
    int existsTransaction(const std::string& sender);

    /**
     * @brief Add a transaction to the pool
     * 
     * @param transaction 
     */
    void addTransaction(Transaction transaction);

    /**
     * @brief Get the Transaction pool
     * 
     * @return std::vector<Transaction> 
     */
    std::vector<Transaction> getTransactions();
};

#endif 