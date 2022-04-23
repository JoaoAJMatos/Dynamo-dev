//
// Created by joaoa on 21/04/2022.
//

#include "NodeClient.h"

/* CONSTRUCTOR */
NodeClient::NodeClient(int domain, int service, int protocol, int port, unsigned long iface) : BasicClient(domain, service, protocol, port, iface)
{
    this->domain = domain;
    this->service = service;
    this->port = port;
    this->iface = iface;
}


/* PUBLIC FUNCTIONS */
char* NodeClient::request(const char *server_ip, const char* buffer)
{
    // Create an address struct for the server
    struct sockaddr_in server_address{};

    server_address.sin_family = this->domain;
    server_address.sin_port = htons(80);
    server_address.sin_addr.s_addr = inet_addr(server_ip);

    inet_pton(this->domain, server_ip, &server_address.sin_addr);

    // Make the connection
    int connection_result = connect(get_socket()->get_sock(), (struct sockaddr*)&server_address, sizeof(server_address));

    // Check for errors
    if (connection_result == 0)
    {
        send(get_socket()->get_sock(), buffer, sizeof(buffer), 0);
        recv(get_socket()->get_sock(), response_buffer, 30000, 0);
        std::cout << response_buffer << std::endl;
    }
    else
    {
        std::cout << "An error has occurred when attempting to connect to the server" << std::endl;
    }

    return nullptr;
}
