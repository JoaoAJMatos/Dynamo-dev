//
// Created by Matos on 3/25/2022.
//

#include "WebServer.h"

/* CONSTRUCTOR */
net::WebServer::WebServer(int domain, int service, int protocol, int port, unsigned long iface, int bklg)
{
    socket = new SocketListener(domain, service, protocol, port, iface, bklg);
}

/* DESTRUCTOR */
net::WebServer::~WebServer() {
    // To prevent memory leaks, we can delete socket once the class is destructed
    delete socket;
}

/* GETTERS */
net::SocketListener *net::WebServer::get_socket() {
    return socket;
}

