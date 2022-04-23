//
// Created by joaoa on 21/04/2022.
//

#ifndef DEV_DYNAMO_NODE_H
#define DEV_DYNAMO_NODE_H

#define UNIX_SERVER_STARTUP_CONFIG_PATH "/etc/dynamo/Node/server/startup.dycfg"
#define WINDOWS_SERVER_STARTUP_CONFIG_PATH "C:\\dynamo\\Node\\server\\startup\\startup.dycfg"
#define UNIX_CLIENT_STARTUP_CONFIG_PATH "/etc/dynamo/Node/client/startup.dycfg"
#define WINDOWS_CLIENT_STARTUP_CONFIG_PATH "C:\\dynamo\\Node\\client\\startup\\startup.dycfg"

#ifdef _WIN32
#define SERVER_STARTUP_CONFIG_PATH WINDOWS_SERVER_STARTUP_CONFIG_PATH
#define CLIENT_STARTUP_CONFIG_PATH WINDOWS_CLIENT_STARTUP_CONFIG_PATH
#else
#define SERVER_STARTUP_CONFIG_PATH UNIX_SERVER_STARTUP_CONFIG_PATH
#define CLIENT_STARTUP_CONFIG_PATH UNIX_CLIENT_STARTUP_CONFIG_PATH
#endif // _WIN32

#define SERVER_CONF_PATH "SERVER_CONF_PATH"
#define CLIENT_CONF_PATH "CLIENT_CONF_PATH"

#include "./common/NodeServer.h"
#include "./common/NodeClient.h"
#include "../../../util/file-handling/config-handling/ConfigParser.h"
#include "../../../util/file-handling/config-handling/StartupConfManager.h"
#include "../../../util/std-out/logger.h"


#include <map>
#include <iostream>

class Node
{
private:
    // Node Server & Client instances
    NodeClient* client;
    NodeServer* server;

public:
    /* CONSTRUCTOR */
    Node();
};


#endif //DEV_DYNAMO_NODE_H
