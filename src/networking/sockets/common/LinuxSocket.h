//
// Created by joaoa on 25/03/2022.
//
// LinuxSocket defines a socket class to be used in Linux systems

#ifdef __linux__

#ifndef DEV_DYNAMO_LINUXSOCKET_H
#define DEV_DYNAMO_LINUXSOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

namespace net
{
    class LinuxSocket {
    private:
        struct sockaddr_in address;
        int sock;
        int connection;

    public:
        LinuxSocket(int domain, int service, int protocol, int port, unsigned long iface);
        virtual void s_connect() = 0;
    };
}


#endif //DEV_DYNAMO_LINUXSOCKET_H

#endif
