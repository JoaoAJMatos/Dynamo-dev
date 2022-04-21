//
// Created by joaoa on 25/03/2022.
//

#include "Socket.h"

/* CONSTRUCTOR */
net::Socket::Socket(int domain, int service, int protocol, int port, u_long iface)
{
    // Define address structure
    address.sin_family = domain;
    address.sin_port = htons(port); // Convert integer byte order to network
    address.sin_addr.s_addr = htonl(iface);

#ifdef _WIN32
    // Attempt to initialize winsock
    WORD wVersionRequired = MAKEWORD(2, 2);
    int iResult = WSAStartup(wVersionRequired, &wsa);

    if (iResult != 0)
    {
        // Tell the user that a usable WinSock DLL was not found
        std::cout << "WSAStartup failed with error: " << WSAGetLastError() << std::endl;
        exit(EXIT_FAILURE);
    }
#endif

    // Establish socket
    sock = socket(domain, service, protocol);
    test_connection(sock);
}

/* PUBLIC FUNCTIONS */
void net::Socket::test_connection(unsigned long item_to_test)
{
#ifdef _WIN32
    // Confirms that either the socket or the connection have been properly established
    // If an error is found, show it and exit the program
    switch (item_to_test)
    {
        case WSANOTINITIALISED:
            perror("WSA not initialized");
            exit(EXIT_FAILURE);
        case WSAENETDOWN:
            perror("Network subsystem failed");
            exit(EXIT_FAILURE);
        case WSAEACCES:
            perror("Socket has no access permissions. Unable to bind a datagram socket to the broadcast address.\nTry enabling SO_BROADCAST in setsockopt.");
            exit(EXIT_FAILURE);
        case WSAEADDRINUSE:
            perror("Address in use. Only one usage of each socket address is permitted.");
            exit(EXIT_FAILURE);
        case WSAEADDRNOTAVAIL:
            perror("Address not valid in the current context. Address is not a valid local IP address on this machine.");
            exit(EXIT_FAILURE);
        case WSAEFAULT:
            perror("Invalid pointer address.");
            exit(EXIT_FAILURE);
        case WSAEINPROGRESS:
            perror("A blocking Windows Sockets call is in progress. Or the service provider is still processing a callback function.");
            exit(EXIT_FAILURE);
        case WSAEINVAL:
            perror("Invalid argument was supplied. Or the socket object is a listening socket.");
            exit(EXIT_FAILURE);
        case WSAENOBUFS:
            perror("Not enough ephemeral ports to allocate for the bind. Or no buffer space is available.");
            exit(EXIT_FAILURE);
        case WSAENOTSOCK:
            perror("Not a socket.");
            exit(EXIT_FAILURE);
        case WSAEAFNOSUPPORT:
            perror("An address with the specified family cannot be used with this socket.");
            exit(EXIT_FAILURE);
        case WSAECONNREFUSED:
            perror("An attempt to connect was forcefully rejected.");
            exit(EXIT_FAILURE);
        case WSAENETUNREACH:
            perror("A socket operation was attempted to an unreachable host.");
            exit(EXIT_FAILURE);
        case WSAETIMEDOUT:
            perror("An attempt to connect timed out without establishing a connection.");
            exit(EXIT_FAILURE);
        case WSAEWOULDBLOCK:
            perror("Socket marked as nonblocking. The connection cannot be completed immediately.");
            exit(EXIT_FAILURE);
        default:
            return;
    }
#endif

    if (item_to_test < 0)
    {
        perror("Connection failed...");
        exit(EXIT_FAILURE);
    }
}

/* GETTERS */
// Returns this socket's address
struct sockaddr_in net::Socket::get_address() {
    return address;
}

// Return this socket
int net::Socket::get_sock() const {
    return sock;
}