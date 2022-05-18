//
// Created by joaoa on 21/04/2022.
//

#ifndef DEV_DYNAMO_NODECLIENT_H
#define DEV_DYNAMO_NODECLIENT_H

#ifndef _WIN32
#include <arpa/inet.h>
#endif

#include <unistd.h>
#include "../../../../networking/objects/BasicClient.h"

class NodeClient
{
private:
    /* MEMBER VARIABLES */
    int domain;
    int service;
    int protocol;

    // Buffer for the incoming server's response
    char response_buffer[30000];

    // Variables regarding the connection with the server
    // The connection will store the return value of the connect() function. (Should be 0)
    int connection;
    bool has_connected;

public:
    /* CONSTRUCTOR */
    NodeClient(int domain, int service, int protocol);

    /* PUBLIC FUNCTIONS */
    // Takes in a server IP and a request, sends the request buffer to the server, waits for a response,
    // and stores it in the response buffer
    int request(const char* server_ip, int server_port, const std::string& request);

    /* GETTERS */
    char* get_response_buffer();
};


#endif //DEV_DYNAMO_NODECLIENT_H
