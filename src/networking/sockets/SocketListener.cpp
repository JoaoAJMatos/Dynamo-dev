//
// Created by joaoa on 28/03/2022.
//

#include "SocketListener.h"

#include "SocketListener.h"

/* CONSTRUCTOR */
net::SocketListener::SocketListener(int domain, int service, int protocol, int port, unsigned long iface, int bklg)
        : SocketBinder(domain, service, protocol, port, iface)
{
    // Set backlog variable
    backlog = bklg;
    // Start listening to the network
    sock_listen();
    // Check for errors
    test_connection(listening);
}

/* MUTATORS */
void net::SocketListener::sock_listen()
{
    listening = listen(get_sock(), backlog);
}

/* GETTERS */
int net::SocketListener::get_backlog() const {
    return backlog;
}

int net::SocketListener::get_listening() const {
    return listening;
}