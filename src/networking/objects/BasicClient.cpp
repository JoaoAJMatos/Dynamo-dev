//
// Created by joaoa on 21/04/2022.
//

#include "BasicClient.h"

BasicClient::BasicClient(int domain, int service, int protocol, int port, unsigned long iface)
{
    // Create a new instance of Socket Connector
    //
    // Since the socket will exist throughout the duration of the node's life we probably
    // don't need to delete sock in order to prevent memory leaks. Not sure tho
    sock = new net::SocketConnector(domain, service, protocol, port, iface);
}

net::SocketConnector* BasicClient::get_socket() {
    return sock;
}
