#include "TransactionPool.h"

#include <utility>

TransactionPool::TransactionPool()
{
    pool.clear();
}

TransactionPool::TransactionPool(std::string transaction_pool_data_packet)
{ 
    std::cout << "[INFO] Transaction pool packet: " << transaction_pool_data_packet << std::endl;

    std::string delimiter = "|";
    std::string buffer = std::move(transaction_pool_data_packet);
    std::map<char*, Transaction*> tempPool;
    std::string transactionString;

    while(!(transactionString = buffer.substr(0, buffer.find('|'))).empty())
    {
        buffer.erase(0, buffer.find(delimiter) + delimiter.length());

        if (transactionString != "-")
        {
            std::cout << "[INFO] Transaction string: " << transactionString << std::endl << std::endl;
            auto* transact = new Transaction(transactionString);
            tempPool[transact->getID()] = transact;
        }
        else break;
    }

    this->pool = tempPool;
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

int TransactionPool::set(std::map<char*, Transaction*> transactionPool)
{
    this->pool = std::move(transactionPool);
    return 0;
}

int TransactionPool::existsTransaction(const std::string& sender)
{
    for (auto [transactionID, transaction] : pool)
    {
        if (transaction->getInputMap().address == sender) return 1;
    }

    return 0;
}

std::vector<Transaction*> TransactionPool::getValidTransactions()
{
    std::vector<Transaction*> validTransactions;

    for (auto item : pool)
    {
        if (Transaction::validTransaction(item.second))
        {
            validTransactions.push_back(item.second);
        }
    }

    return validTransactions;
}

void TransactionPool::clearBlockchainTransactions(Blockchain* chain)
{
    for (auto& block : chain->chain)
    {
        if (block->getHeight() == 0) continue; // Skip the Genesis block

        for (auto transaction : block->data)
        {
            if (this->pool[transaction->getID()]) delete this->pool[transaction->getID()];
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

std::string TransactionPool::toString(TransactionPool* pool)
{
    std::stringstream ss;
    for (auto item : pool->getPool())
    {
        ss << Transaction::toString(item.second) << "|";
    }

    ss << "-";

    return ss.str();
}

void TransactionPool::show()
{
    if (this->pool.empty())
    {
        std::cout << "[INFO] The transaction pool is empty." << std::endl;
        return;
    }

    for (auto item : pool)
    {
        item.second->showTransaction();
    }
}

std::map<char*, Transaction*> TransactionPool::getPool()
{
    return pool;
}