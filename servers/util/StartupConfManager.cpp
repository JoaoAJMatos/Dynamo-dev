//
// Created by joaoa on 18/04/2022.
//

#include "StartupConfManager.h"

int server_utils::set_startup_config(const char *name, const char *value)
{
    // Create and open the file
    std::ofstream startup_config_file(STARTUP_CONFIG_PATH);

    startup_config_file << name << "=" << value << std::endl;

    return 0;
}

