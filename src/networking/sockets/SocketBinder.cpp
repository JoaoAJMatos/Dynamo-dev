//
// Created by joaoa on 25/03/2022.
//

#include "SocketBinder.h"

/* CONSTRUCTOR */
net::SocketBinder::SocketBinder(int domain, int service, int protocol, int port, u_long iface)
                  : Socket(domain, service, protocol, port, iface)
{
    // Establish network connection
    sock_connect(get_sock(), get_address());
    // Test the connection
    test_connection(binding);
}

/* VIRTUAL FUNCTION OVERRIDE */
// Definition of connect() virtual function
// This function overrides the Socket.h's connect() and defines it to use
// the bind() function from winsock2.h
void net::SocketBinder::sock_connect(int sock, struct sockaddr_in address)
{
    binding = bind(sock, (struct sockaddr *)&address, sizeof(address));
}

/* GETTERS */
int net::SocketBinder::get_binding() const
{
    return binding;
}
