//
// Created by joaoa on 21/04/2022.
//
// Here will be defined the inner workings of the server part of the node.
// Each node will run a server in a separate thread which will handle incoming connections from other nodes.
// It's the server's responsibility to handle the connections and tell the Node how it should react to each interaction.

#ifndef DEV_DYNAMO_NODESERVER_H
#define DEV_DYNAMO_NODESERVER_H

#define BUFFER_SIZE 300000
#define PACKET_SIZE 1024

#define ACK -1
#define BLOCKCHAIN_DATA_PACKET 0
#define BLOCKCHAIN_REQUEST_PACKET 1
#define TRANSACTION_PACKET 2
#define TRANSACTION_POOL_DATA_PACKET 3
#define TRANSACTION_POOL_REQUEST_PACKET 4

#include <unistd.h>
#include <fstream>

#ifndef _WIN32
#include <arpa/inet.h>
#endif

#include <fcntl.h>

#include "../../../../networking/objects/BasicServer.h"
#include "../../../wallet/transaction/TransactionPool.h"
#include "../../../blockchain/Blockchain.h"
#include "../../protocol/DTP.h"
#include "../../../../system/threading/ThreadPool.h"
#include "../../../../util/std-out/logger.h"
#include "../../../../../libs/msgpack11/msgpack11.hpp"

class NodeServer : public net::BasicServer
{
private:
    /* MEMBER VARIABLES */
    // Variables regarding the server
    int domain;
    int service;
    int protocol;
    int port;
    unsigned long iface;
    int backlog;

    // Multi threading and thread pool instance
    int number_of_threads;
    sys::ThreadPool* tp;

    DTP::Packet* packet;
    char* nodeIP;
    int nodePort;

    // References to Node variables
    int* broadcast_flag;
    std::string* broadcast_buffer; 
    Blockchain* blockchain;
    TransactionPool* transactionPool;
    std::vector<std::pair<std::string, int>>* known_hosts;
    char* uuid;

    // Buffer to store the request
    char buffer[BUFFER_SIZE];

    // New socket descriptor for the incoming connections
    unsigned long new_socket;

    // Map of subscribed broadcast channels.
    // The map will indicate which channels the node should listen to

public:
    /* CONSTRUCTOR / DESTRUCTOR */
    NodeServer(int domain, int service, int protocol, int port, unsigned long iface, int bklg, int thread_count = DEFAULT_THREAD_COUNT);
    ~NodeServer();

    /* PUBLIC FUNCTIONS & FUNCTION OVERRIDES */
    void accepter() override;
    void handler() override;
    void responder() override;

    void launch() override;

    int isKnownHost(std::string ip, int port);

    int send_file(FILE* fp, int socket);
    int receive_file(int sockfd);
    int ftp_transfer(std::string payload);

    /* GETTERS/SETTERS */
    void set_working_blockchain(Blockchain* blockchain);
    void set_working_transaction_pool(TransactionPool* transactionPool);
    void set_node_uuid(char* uuid);
    void set_known_hosts(std::vector<std::pair<std::string, int>>* known_hosts);
};


#endif //DEV_DYNAMO_NODESERVER_H
