//
// Created by joaoa on 05/04/2022.
//

#include "DNS_Server.h"

/* CONSTRUCTOR/DESTRUCTOR */
servers::DNS_Server::DNS_Server(int domain, int service, int protocol, int port, unsigned long iface, int bklg, int thread_count)
: net::BasicServer(domain, service, protocol, port, iface, bklg)
{
    // Set member variables
    this->domain = domain;
    this->service = service;
    this->protocol = protocol;
    this->port = port;
    this->iface = iface;
    this->backlog = bklg;
    this->number_of_threads = thread_count;

    // Create the thread pool
    tp = new sys::ThreadPool(number_of_threads);
}

servers::DNS_Server::~DNS_Server()
{
    delete tp;
}

/* MEMBER FUNCTIONS */
void servers::DNS_Server::accepter()
{
    // Socket address struct
    struct sockaddr_in address = get_socket()->get_address();

    // Length of the address struct to be passed into the accept() function
    int addrlen = sizeof(address);

    // Store the new socket
    new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrlen);

    // Test the connection for errors
    net::Socket::test_connection(new_socket);

    // Read the incoming message and store it in the buffer
    read(new_socket, buffer, BUFFER_SIZE);
}

// The handler function is responsible for handling the incoming request and parsing it to create
// a new DNS request class instance.
void servers::DNS_Server::handler()
{
    DNS_query = new net::DNS(buffer);
}

// The responder will analyze the DNS query and respond accordingly
void servers::DNS_Server::responder()
{
    std::string message_buffer;

    // If the server recieves a SYNC-ME type query:
    // Create a message buffer containing all the known hosts and send it to the client
    if (DNS_query->get_type() == SYNC_ME)
    {
        // Create message buffer
        net_utils::map_to_buffer(known_hosts);
    }
}

/* PUBLIC FUNCTIONS */
// This function launches the server
void servers::DNS_Server::launch()
{
    tp->do_work([&]() {
        accepter();
        handler();
    });
}

/* GETTERS */
// Returns a dictionary containing all the known hosts
std::map<std::string, std::string> servers::DNS_Server::get_known_hosts()
{
    return known_hosts;
}

// Returns a dictionary containing all the active connections
std::map<std::string, std::time_t> servers::DNS_Server::get_connection_table()
{
    return connection_table;
}






