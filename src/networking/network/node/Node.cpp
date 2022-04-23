//
// Created by joaoa on 21/04/2022.
//

#include "Node.h"

/* CONSTRUCTOR */
Node::Node()
{
    // Fetch the config file
    std::map<std::string, std::string> server_startup_config;
    server_startup_config = config::getConfigFromFile(SERVER_STARTUP_CONFIG_PATH);

    std::map<std::string, std::string> client_startup_config;
    server_startup_config = config::getConfigFromFile(CLIENT_STARTUP_CONFIG_PATH);

    if(server_startup_config.find(SERVER_CONF_PATH) != server_startup_config.end())
    {

    }
    else
    {

    }
}
