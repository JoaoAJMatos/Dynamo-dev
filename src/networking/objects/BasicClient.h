//
// Created by joaoa on 21/04/2022.
//
// The BasicClient Class serves as the skeleton for making a client in a client-server architecture

#ifndef DEV_DYNAMO_BASICCLIENT_H
#define DEV_DYNAMO_BASICCLIENT_H

#include "../sockets/SocketConnector.h"

class BasicClient
{
private:
    /* MEMBER VARIABLES */
    net::SocketConnector* sock;

public:
    /* CONSTRUCTOR */
    BasicClient(int domain, int service, int protocol, int port, unsigned long iface);

    /* VIRTUAL FUNCTIONS */
    virtual char* request(const char* server_ip, const char* request) = 0;

    /* GETTERS */
    net::SocketConnector* get_socket();
};


#endif //DEV_DYNAMO_BASICCLIENT_H
