//
// Created by joaoa on 07/05/2022.
//

#include "exprFinder.h"

bool exists_in_file(char* fileName, char* expression)
{
    std::string line;
    std::ifstream file;

    // Open the file
    file.open(fileName);

    if(file.is_open())
    {
        while(!file.eof())
        {
            getline(file, line);

            if((line.find(expression, 0)) != std::string::npos)
            {
                file.close();
                return true;
            }
        }

        file.close();
    }

    return false;
}

