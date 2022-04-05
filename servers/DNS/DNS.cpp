//
// Created by joaoa on 05/04/2022.
//

#include "DNS.h"

/* CONSTRUCTOR */
servers::DNS::DNS(int domain, int service, int protocol, int port, unsigned long iface, int bklg): net::BasicServer(domain, service, protocol, port, iface, bklg)
{

}
