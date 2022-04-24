//
// Created by joaoa on 21/04/2022.
//

#include "NodeClient.h"

/* CONSTRUCTOR */
NodeClient::NodeClient(int domain, int service, int protocol)
{
    // Set server variables
    this->domain = domain;
    this->service = service;
    this->protocol = protocol;

    this->has_connected = false;

#ifdef _WIN32
    // Startup WSA if on Windows
    WSADATA wsa;
    WORD wVersionRequired = MAKEWORD(2, 2);
    int iResult = WSAStartup(wVersionRequired, &wsa);
#endif
}


/* PUBLIC FUNCTIONS */
char* NodeClient::request(const char *server_ip, const int server_port, const char* buffer)
{
    // Socket file descriptor
    int sock;

    // Create the socket
    sock = socket(this->domain, this->service, this->protocol);

    // Create an address struct for the server
    struct sockaddr_in server_address{};

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    server_address.sin_addr.s_addr = inet_addr(server_ip);

    inet_pton(this->domain, server_ip, &server_address.sin_addr);

    connection = connect(sock, (struct sockaddr*)&server_address, sizeof(server_address));

    // Check for errors
    if (connection == 0)
    {
        send(sock, buffer, 30000, 0);
        recv(sock, response_buffer, 30000, 0);
        std::cout << response_buffer << std::endl;
    }
    else
    {
        std::cout << "An error has occurred when attempting to connect to the server" << std::endl;
        std::cout << WSAGetLastError();
    }

    return nullptr;
}
