//
// Created by joaoa on 28/03/2022.
//

#include "BasicServer.h"

/* CONSTRUCTOR */
net::BasicServer::BasicServer(int domain, int service, int protocol, int port, unsigned long iface, int bklg)
{
    socket = new SocketListener(domain, service, protocol, port, iface, bklg);
}

/* DESTRUCTOR */
net::BasicServer::~BasicServer() {
    // To prevent memory leaks, we can delete socket once the class is destructed
    delete socket;
}

/* GETTERS */
net::SocketListener *net::BasicServer::get_socket() {
    return socket;
}