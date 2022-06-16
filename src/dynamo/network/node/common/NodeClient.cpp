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
int NodeClient::request(const char *server_ip, const int server_port, const std::string& buffer)
{
    response_string.clear();

    // Create the socket
    this->sock = socket(this->domain, this->service, this->protocol);

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
        int bytes = send(sock, buffer.data(), buffer.size(), 0);
        int res = 1;

        res = recv(sock, response_buffer, 300000, 0);

        if (res < 0)
        {
            std::cout << "[+] Error receiving response from server" << std::endl;
            return -1;
        }
    }
    else
    {
        std::cout << std::endl << "[ERROR] An error has occurred when attempting to connect to the server" << std::endl;

#ifdef _WIN32
        std::cout << "[ERROR] WSA error code: " << WSAGetLastError() << std::endl;
#endif

        return -1;
    }

    return 0;
}

/* GETTERS */
char* NodeClient::get_response_buffer()
{
    return response_buffer;
}

int NodeClient::get_sock()
{
    return sock;
}