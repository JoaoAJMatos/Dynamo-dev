//
// Created by joaoa on 25/03/2022.
//

#ifndef DEV_DYNAMO_SOCKETCONNECTOR_H
#define DEV_DYNAMO_SOCKETCONNECTOR_H

#include "common/Socket.h"

namespace net
{
    class SocketConnector: public Socket
    {
    private:
        /* MEMBER VARIABLES */
        // The connection variable will store the result of the connect function defined in Socket.h
        // Holds the value 0 if no error occurs while connecting
        int connection;

    public:
        /* CONSTRUCTOR */
        SocketConnector(int domain, int service, int protocol, int port, unsigned long iface);

        /* VIRTUAL FUNCTION OVERRIDES */
        int sock_connect(int sock, struct sockaddr_in address) override;

        /* GETTERS */
        int get_connection() const;
    };
}


#endif //DEV_DYNAMO_SOCKETCONNECTOR_H
