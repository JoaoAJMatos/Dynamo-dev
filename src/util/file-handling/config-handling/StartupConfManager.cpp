//
// Created by joaoa on 18/04/2022.
//

#include "StartupConfManager.h"

int config::set_config(const std::string& path, const std::string& filename ,const std::string& name, const std::string& value)
{
    // C++ doesn't have standard way to create a directory, therefore, ofstream can't open a file if the directories aren't created.
    // In order to write to the file and save the configs we must first check if the directory exists, and if not, create it

    // ======= CHECK IF DIRECTORY EXISTS =======
    // Check if the path refers to an existing file system object
    if (!(std::filesystem::exists(path)))
    {
        // Return if the directory couldn't be created
        if(!(std::filesystem::create_directory(path))) return -1;
    }

    // Create and open the file
    std::ofstream startup_config_file(path + "\\" + filename); // Append the filename to the path

    // Return if the file couldn't be opened
    if (!startup_config_file) return -1;

    startup_config_file << name << "=" << value << std::endl;

    startup_config_file.close();

    return 0;
}

