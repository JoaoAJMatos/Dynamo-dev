//
// Created by joaoa on 25/03/2022.
//

#include "SocketConnector.h"

/* CONSTRUCTOR */
net::SocketConnector::SocketConnector(int domain, int service, int protocol, int port, unsigned long iface)
                     : Socket(domain, service, protocol, port, iface)
{
    // Establish network connection
    sock_connect(get_sock(), get_address());
    // Test the connection
    test_connection(connection);
}


/* VIRTUAL FUNCTION OVERRIDES */
// Definition of connect() virtual function declared in Socket.h
void net::SocketConnector::sock_connect(int sock, struct sockaddr_in address) {
    // Sets the connection variable to the result of the connection. 0 if successful
    connection = connect(sock, (struct sockaddr*)&address, sizeof(address));
}


/* GETTERS */
int net::SocketConnector::get_connection() const {
    return connection;
}
