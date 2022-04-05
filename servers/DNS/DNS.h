//
// Created by joaoa on 05/04/2022.
//

#ifndef DEV_DYNAMO_DNS_H
#define DEV_DYNAMO_DNS_H

#include "../../src/networking/server/servers.h"
#include "../../src/system/threading/ThreadPool.h"

namespace servers
{
    class DNS: public net::BasicServer
    {
    private:
        /* MEMBER VARIABLES */


    public:
        /* CONSTRUCTOR */
        DNS(int domain, int service, int protocol, int port, unsigned long iface, int bklg);
    };
}


#endif //DEV_DYNAMO_DNS_H
