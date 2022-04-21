//
// Created by joaoa on 21/04/2022.
//

#include "NodeServer.h"

/* CONSTRUCTOR */
NodeServer::NodeServer(int domain, int service, int protocol, int port, unsigned long iface, int bklg,
                       int thread_count) : net::BasicServer(domain, service, protocol, port, iface, bklg)
{
    // Set Node server variables
    this->domain = domain;
    this->service = service;
    this->protocol = protocol;
    this->port = port;
    this->iface = iface;
    this->backlog = bklg;
    this->number_of_threads = thread_count;

    // Call the constructor for the thread pool
    tp = new sys::ThreadPool(number_of_threads);
}

/* DESTRUCTOR */
NodeServer::~NodeServer()
{
    delete tp;
}

/* PUBLIC FUNCTIONS AND FUNCTION OVERRIDES */
void NodeServer::accepter()
{
    struct sockaddr_in address = get_socket()->get_address();

    // length of the address struct to be passed into the accept() function
    int addrlen = sizeof(address);

    // Store the new socket descriptor
    new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
    net::Socket::test_connection(new_socket);

    // Read the incoming message and store it in the buffer
    recv(new_socket, buffer, BUFFER_SIZE, 0);
}
