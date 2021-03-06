//
// Created by joaoa on 28/03/2022.
//

#ifndef DEV_DYNAMO_SOCKETLISTENER_H
#define DEV_DYNAMO_SOCKETLISTENER_H

#include "SocketBinder.h"

namespace net
{
    class SocketListener: public SocketBinder {
    private:
        /* MEMBER VARIABLES */
        // A server must have a backlog of type int.
        // This variable will store the max permitted number of pending connections the queue will hold.
        // If the queue is full, further incoming connections will be rejected
        int backlog;

        // Will hold the result for the listen() socket call. Must be 0 if no errors occur
        int listening;

    public:
        /* CONSTRUCTOR */
        SocketListener(int domain, int service, int protocol, int port, unsigned long iface, int bklg);

        /* PUBLIC FUNCTIONS */
        void sock_listen();

        /* GETTERS */
        [[nodiscard]] int get_backlog() const;
        [[nodiscard]] int get_listening() const;
    };
}

#endif //DEV_DYNAMO_SOCKETLISTENER_H
