//
// Created by Matos on 3/25/2022.
//

#include "SocketListener.h"

/* CONSTRUCTOR */
net::SocketListener::SocketListener(int domain, int service, int protocol, int port, unsigned long iface, int bklg)
: SocketBinder(domain, service, protocol, port, iface)
{
    // Set backlog variable
    backlog = bklg;
    // Start listening to the network
    sock_listen();
}

/* PUBLIC FUNCTIONS */
void net::SocketListener::sock_listen()
{

}

/* GETTERS */
int net::SocketListener::get_backlog() const {
    return backlog;
}

int net::SocketListener::get_listening() const {
    return listening;
}
