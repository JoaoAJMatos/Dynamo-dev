#include "TransactionPool.h"

void TransactionPool::clear()
{
    pool.clear();
}
/*
int TransactionPool::setTransaction(Transaction transaction)
{
    pool[transaction.getID()] = transaction;
    return 0;
}

int TransactionPool::set(std::vector<Transaction> pool)
{
    this->validTransactions = pool;
    return 0;
}

int TransactionPool::existsTransaction(const std::string& sender)
{
    for (auto& [transactionID, transaction] : pool)
    {
        if (transaction.getInputMap().address == sender) return 1;
    }

    return 0;
}

void TransactionPool::addTransaction(Transaction transaction)
{
    validTransactions.push_back(transaction);
}

std::vector<Transaction> TransactionPool::getValidTransactions()
{
    for (auto& [transactionID, transaction] : pool)
    {
        if (Transaction::validTransaction(&transaction))
        {
            validTransactions.push_back(transaction);
        }
    }

    return this->validTransactions;
}*/