//
// Created by joaoa on 18/04/2022.
//

#include "StartupConfManager.h"

int config::set_config(const std::string& path, const std::string& name, const std::string& value, bool append)
{
    // The set_config() function assumes that the directory tree has been created for the incoming path
    // Therefore, all the directories must be created before calling this function

    // Create and open the file
    std::ofstream config_file;

    if (append) config_file.open(path, std::ios_base::app); // Open ofstream in append mode
    else config_file.open(path);

    // Return if the file couldn't be opened
    if (!config_file) return -1;

    config_file << name << "=" << value << std::endl;

    config_file.close();

    return 0;
}

