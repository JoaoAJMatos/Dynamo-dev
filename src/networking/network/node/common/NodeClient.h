//
// Created by joaoa on 21/04/2022.
//

#ifndef DEV_DYNAMO_NODECLIENT_H
#define DEV_DYNAMO_NODECLIENT_H

#include <unistd.h>
#include "../../../objects/BasicClient.h"

class NodeClient : public BasicClient
{
private:
    /* MEMBER VARIABLES */
    int domain;
    int service;
    int protocol;
    int port;
    unsigned long iface;

    char response_buffer[30000];

public:
    /* CONSTRUCTOR */
    NodeClient(int domain, int service, int protocol, int port, unsigned long iface);

    char* request(const char *server_ip, const char* buffer) override;
};


#endif //DEV_DYNAMO_NODECLIENT_H
