//
// Created by joaoa on 18/04/2022.
//

#include "StartupConfManager.h"

int config::set_config(const std::string& path ,const char *name, const char *value)
{
    // Create and open the file
    std::ofstream startup_config_file(path);

    startup_config_file << name << "=" << value << std::endl;

    return 0;
}

