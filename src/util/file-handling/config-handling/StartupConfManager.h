//
// Created by joaoa on 18/04/2022.
//
// The following code handles reading and writing to the startup configuration file of a server
// The startup configuration file has information regarding the location of the config file of a specific server
// as well as other configs. This file is stored on different locations depending on the host OS.

#ifndef DEV_DYNAMO_STARTUPCONFMANAGER_H
#define DEV_DYNAMO_STARTUPCONFMANAGER_H

#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <map>

#include "ConfigParser.h"

namespace config
{
    // Set a configuration in a file in the format name=data
    int set_config(const std::string& path, const std::string& name, const std::string& value, bool append);
}

#endif //DEV_DYNAMO_STARTUPCONFMANAGER_H
