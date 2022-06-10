#include "TransactionPool.h"

TransactionPool::TransactionPool()
{
    pool.clear();
}

TransactionPool::TransactionPool(std::string transaction_pool_data_packet)
{
    using namespace msgpack11;
    pool.clear();

    // Deserialize the transaction packet
    std::string err;
    MsgPack transaction_pool = MsgPack::parse(transaction_pool_data_packet, err);

    try
    {
        MsgPack::array transactions = transaction_pool["pool"].array_items();

        for (auto& element : transactions)
        {
            Transaction* transaction = new Transaction(element.dump());
            this->pool[transaction->getID()] = transaction;
        }
    }
    catch(const std::exception& e)
    {
        std::cout << "[ERROR] Unable to create a transaction-pool instance from the incomming packet." << std::endl;
    }
    
}

void TransactionPool::clear()
{
    pool.clear();
}

int TransactionPool::setTransaction(Transaction* transaction)
{
    pool[transaction->getID()] = transaction;
    return 0;
}

int TransactionPool::set(std::map<char*, Transaction*> pool)
{
    this->pool = pool;
    return 0;
}

int TransactionPool::existsTransaction(const std::string& sender)
{
    for (auto& [transactionID, transaction] : pool)
    {
        if (transaction->getInputMap().address == sender) return 1;
    }

    return 0;
}

std::vector<Transaction> TransactionPool::getValidTransactions()
{
    std::vector<Transaction> validTransactions;

    for (auto& [transactionID, transaction] : pool)
    {
        if (Transaction::validTransaction(transaction))
        {
            validTransactions.push_back(*transaction);
        }
    }

    return validTransactions;
}

void TransactionPool::clearBlockchainTransactions(Blockchain* chain)
{
    for (auto& block : chain->chain)
    {
        if (block->getHeight() == 0) continue; // Skip the Genesis block

        for (auto& transaction : block->data)
        {
            if (this->pool[transaction.getID()]) delete this->pool[transaction.getID()];
        }
    }
}

msgpack11::MsgPack TransactionPool::serialize(TransactionPool* pool)
{
    using namespace msgpack11;

    MsgPack serialized;
    MsgPack::array poolArray;

    for (auto& [transactionID, transaction] : pool->getPool())
    {
        poolArray.push_back(Transaction::serialize(transaction));
    }

    serialized = MsgPack::object{{"pool", poolArray}};

    return serialized;
}

std::map<char*, Transaction*> TransactionPool::getPool()
{
    return pool;
}