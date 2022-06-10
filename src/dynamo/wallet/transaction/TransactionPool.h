#ifndef DEV_DYNAMO_TRANSACTION_POOL_H
#define DEV_DYNAMO_TRANSACTION_POOL_H

#include <vector>
#include <map>

#include "Transaction.h"
#include "../../blockchain/Blockchain.h"
#include "../../../../libs/msgpack11/msgpack11.hpp"

class TransactionPool
{
private:
    std::map<char*, Transaction*> pool;

public:
    TransactionPool();
    TransactionPool(std::string transaction_pool_data_packet); // Build a transaction pool object from an incoming DTP packet

    /**
     * @brief Clear the transaction pool
     * 
     */
    void clear();

    int setTransaction(Transaction* transaction);

    /**
     * @brief Set this transaction pool to an incomming one
     * 
     * @param pool 
     * @return int 
     */
    int set(std::map<char*, Transaction*> pool);

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

    void clearBlockchainTransactions(Blockchain* chain);

    static msgpack11::MsgPack serialize(TransactionPool* pool);

    /* GETTERS */
    std::map<char*, Transaction*> getPool();
};

#endif 