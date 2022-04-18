//
// Created by joaoa on 28/03/2022.
//

#ifndef DEV_DYNAMO_BASICSERVER_H
#define DEV_DYNAMO_BASICSERVER_H


#include "../sockets/SocketListener.h"

namespace net
{
    class BasicServer
    {
    private:
        /* MEMBER VARIABLES */
        // A pointer to a listening socket
        SocketListener* socket;

        /* VIRTUAL MEMBER FUNCTIONS */
        // To avoid specifying return types and parameters, the following functions must be defined inside
        // the child classes. You can define the functions in the child class in order for the server to fit
        // your needs, handling connections, requests and responses however you want.
        virtual void accepter() = 0;
        virtual void handler() = 0;
        virtual void responder() = 0;


    public:
        /* CONSTRUCTOR */
        BasicServer(int domain, int service, int protocol, int port, unsigned long iface, int bklg);

        /* VIRTUAL FUNCTIONS */
        // The launch function makes a call to the virtual member functions defined inside the child class.
        virtual void launch() = 0;

        /* GETTERS */
        SocketListener* get_socket();
    };
}


#endif //DEV_DYNAMO_BASICSERVER_H
