//
// Created by joaoa on 18/04/2022.
//
// The code below parses a config file. Removes all the whitespaces from the line and skips the commented lines (#).
// Splits the string at the delimiter and returns the name and value.

#ifndef DEV_DYNAMO_CONFIGPARSER_H
#define DEV_DYNAMO_CONFIGPARSER_H

#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>

namespace server_utils
{
    std::map<std::string, std::string> getConfigFromFile(const std::string& filePath);
}

#endif //DEV_DYNAMO_CONFIGPARSER_H
