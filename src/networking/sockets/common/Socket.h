//
// Created by joaoa on 25/03/2022.
//

#ifdef _WIN32

#ifndef DEV_DYNAMO_SOCKET_H
#define DEV_DYNAMO_SOCKET_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <winerror.h>
#include <tchar.h>
#include <iostream>

namespace net
{
    class Socket
    {
    private:
        /* MEMBER VARIABLES */
        // Once instantiated, sock contains the address of a socket object on the network.
        // This should be a non-negative number.
        int sock;

        // The address variable is a struct that contains information about the protocol,
        // type, and interface of a given socket. These parameters are specified and set in the constructor
        struct sockaddr_in address;

        // Struct that contains information about the windows sockets implementation
        WSADATA wsa;

    public:
        /* CONSTRUCTOR */
        // A parameterized constructor is required to instantiate the class.
        Socket(int domain, int service, int protocol, int port, u_long iface);

        /* VIRTUAL FUNCTIONS */
        // This function connects to the network using either bind() or connect() from winsock2.h
        virtual int connect(int sock, struct sockaddr_in address) = 0;

        /* TESTERS */
        // This function exits the program if the connection is not properly established.
        // This should be checked frequently.
        static void test_connection(int);

        /* GETTERS */
        struct sockaddr_in get_address();
        int get_sock() const;
    };
}

#endif //DEV_DYNAMO_SOCKET_H

#endif