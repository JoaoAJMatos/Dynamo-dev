//
// Created by joaoa on 25/03/2022.
//

#ifdef __linux__

#include "LinuxSocket.h"

net::LinuxSocket::LinuxSocket(int domain, int service, int protocol, int port, unsigned long iface)
{
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(iface);
    sock = socket(domain, service, protocol);
}

#endif
