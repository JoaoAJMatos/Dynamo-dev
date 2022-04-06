//
// Created by joaoa on 05/04/2022.
//

#include "DNS.h"

/* CONSTRUCTOR */
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

/* PUBLIC FUNCTIONS */


/* GETTERS */
// Returns a dictionary containing all the known hosts
std::map<boost::uuids::uuid, sockaddr_in> servers::DNS::get_known_hosts()
{
    return known_hosts;
}

// Returns a dictionary containing all the active connections
std::map<boost::uuids::uuid, std::time_t> servers::DNS::get_connection_table() {
    return connection_table;
}
