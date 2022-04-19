//
// Created by joaoa on 18/04/2022.
//
// The following code handles reading and writing to the startup configuration file of a server
// The startup configuration file has information regarding the location of the config file of a specific server
// as well as other configs. This file is stored on different locations depending on the host OS.

#ifndef DEV_DYNAMO_STARTUPCONFMANAGER_H
#define DEV_DYNAMO_STARTUPCONFMANAGER_H

#define UNIX_STARTUP_CONFIG_PATH "/etc/dynamo/DDNS/startup.dycfg"
#define WINDOWS_STARTUP_CONFIG_PATH "C:\\dynamo\\ddns\\startup\\startup.dycfg"

#ifdef _WIN32
#define STARTUP_CONFIG_PATH WINDOWS_STARTUP_CONFIG_PATH
#else
#define STARTUP_CONFIG_PATH UNIX_STARTUP_CONFIG_PATH
#endif // _WIN32

#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <map>

#include "ConfigParser.h"

namespace server_utils
{
    int set_config(const std::string& path ,const char* name, const char* value);
}

#endif //DEV_DYNAMO_STARTUPCONFMANAGER_H
