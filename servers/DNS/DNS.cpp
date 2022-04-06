//
// Created by joaoa on 05/04/2022.
//

#include "DNS.h"

/* CONSTRUCTOR/DESTRUCTOR */
servers::DNS::DNS(int domain, int service, int protocol, int port, unsigned long iface, int bklg, int thread_count)
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

servers::DNS::~DNS()
{
    delete tp;
}

/* MEMBER FUNCTIONS */
void servers::DNS::accepter()
{
}

/* PUBLIC FUNCTIONS */
// This function launches the server
void servers::DNS::launch()
{
    tp->do_work([&]() {
        accepter();
    });
}

/* GETTERS */
// Returns a dictionary containing all the known hosts
std::map<char*, sockaddr_in> servers::DNS::get_known_hosts()
{
    return known_hosts;
}

// Returns a dictionary containing all the active connections
std::map<char*, std::time_t> servers::DNS::get_connection_table()
{
    return connection_table;
}


