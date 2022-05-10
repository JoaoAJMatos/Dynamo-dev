//
// Created by joaoa on 21/04/2022.
//

#ifndef DEV_DYNAMO_NODE_H
#define DEV_DYNAMO_NODE_H

#define STARTUP_CONFIG_FILE_NAME "startup.dycfg"
#define NODE_CONFIG_FILE_NAME "node.conf"
#define UNIX_NODE_STARTUP_CONFIG_PATH "/etc/dynamo/Node/startup"
#define DEFAULT_UNIX_NODE_CONFIG_PATH "/etc/dynamo/Node"
#define WINDOWS_NODE_STARTUP_CONFIG_PATH "C:\\dynamo\\Node\\startup"
#define FULL_STARTUP_PATH_WINDOWS "C:\\dynamo\\Node\\startup\\startup.dycfg"
#define FULL_STARTUP_PATH_UNIX "/etc/dynamo/Node/startup.dycfg"
#define DEFAULT_WINDOWS_NODE_CONFIG_PATH "C:\\dynamo\\Node"
#define CONFIG_PATH "NODE_CONFIG_FILE"

#ifdef _WIN32
#define NODE_STARTUP_CONFIG_PATH WINDOWS_NODE_STARTUP_CONFIG_PATH
#define DEFAULT_CONFIG_PATH DEFAULT_WINDOWS_NODE_CONFIG_PATH
#define FULL_STARTUP_PATH FULL_STARTUP_PATH_WINDOWS
#else
#define NODE_STARTUP_CONFIG_PATH UNIX_NODE_STARTUP_CONFIG_PATH
#define DEFAULT_CONFIG_PATH DEFAULT_UNIX_NODE_CONFIG_PATH
#define FULL_STARTUP_PATH FULL_STARTUP_PATH_UNIX
#endif // _WIN32

#define MAX_BUFFER_LENGTH 512
#define DDNS1 "127.0.0.1" // Name server 1's IP
#define DDNS1_PORT 4542
#define MAX_ATTEMPTS 3

#include "./common/NodeServer.h"
#include "./common/NodeClient.h"
#include "../../protocols/DDNS/DNS.h"
#include "../../../util/file-handling/config-handling/ConfigParser.h"
#include "../../../util/file-handling/config-handling/StartupConfManager.h"
#include "../../../util/std-out/logger.h"
#include "../../../util/uuid/uuid.h"
#include "../../../system/time/Time.h"

#include <map>
#include <iostream>

class Node
{
private:
    /* MEMBER VARIABLES */

    // Variables regarding both socket descriptors
    // since both the server and the client use the same address family, service, and protocol
    int domain;
    int service;
    int protocol;
    int server_port;
    unsigned long server_interface;
    int backlog;
    int number_of_threads;
    char uuid[38];

    // Map of all the known hosts
    // This map will store the IP address and port of all the server nodes
    // This map will be filled in after the node syncs its state with the name server
    std::map<std::string, int> known_hosts;

    // Node Server & Client instances
    // These instances will be launched in separate threads
    //
    NodeClient* client;     // Launched on the main thread
    NodeServer* server;     // Launched on the second thread

    /* PRIVATE FUNCTIONS */
    // This function will save the server and client configurations
    void save_config(const std::string& config_file_path) const;

public:
    /* CONSTRUCTOR/DESTRUCTOR */
    // The constructor takes in the configuration for the server and client instances
    Node();
    ~Node();


    /* PUBLIC FUNCTIONS */
    // This function will fetch the known hosts list from the default name server
    int discover_peers();

    void start();
};


#endif //DEV_DYNAMO_NODE_H
