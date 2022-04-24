//
// Created by joaoa on 21/04/2022.
//

#ifndef DEV_DYNAMO_NODECLIENT_H
#define DEV_DYNAMO_NODECLIENT_H

#include <unistd.h>
#include "../../../objects/BasicClient.h"

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
    char* request(const char* server_ip, const int server_port, const char* request);
};


#endif //DEV_DYNAMO_NODECLIENT_H
