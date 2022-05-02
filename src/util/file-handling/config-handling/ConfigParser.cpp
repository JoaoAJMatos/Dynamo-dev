//
// Created by joaoa on 18/04/2022.
//

#include "ConfigParser.h"

std::map<std::string, std::string> config::getConfigFromFile(const std::string &filePath)
{
    // Create a map for the output
    std::map<std::string, std::string> configMap;

    // std::ifstream is RAII, therefore we don't need to call close()
    std::ifstream confFile(filePath);

    if(confFile.is_open())
    {
        std::string line;

        while(getline(confFile, line))
        {
            line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());

            if(line[0] == '#' || line.empty()) continue;

            auto delimiterPos = line.find('=');
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);

            // Insert the name and value into the map
            configMap.insert({name, value});
        }
    }

    confFile.close();

    return configMap;
}
