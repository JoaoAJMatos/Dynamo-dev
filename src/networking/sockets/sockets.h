//
// Created by joaoa on 25/03/2022.
//

#ifndef DEV_DYNAMO_SOCKETS_H
#define DEV_DYNAMO_SOCKETS_H

// Import different header files for Linux and Windows
#ifdef __linux__
#include "common/LinuxSocket.h"
#else
#include "common/Socket.h"
#endif

#include "SocketBinder.h"

#endif //DEV_DYNAMO_SOCKETS_H
