//
// Created by joaoa on 21/04/2022.
//
// Here will be defined the inner workings of the server part of the node.
// Each node will run a server in a separate thread which will handle incoming connections from other nodes.
// It's the server's responsibility to handle the connections and tell the Node how it should react to each interaction.

#ifndef DEV_DYNAMO_NODESERVER_H
#define DEV_DYNAMO_NODESERVER_H

#define BUFFER_SIZE 30000

#include "../../../objects/BasicServer.h"
#include "../../../../system/threading/ThreadPool.h"
#include "../../../../util/std-out/logger.h"

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

    /* GETTERS/SETTERS */

};


#endif //DEV_DYNAMO_NODESERVER_H
