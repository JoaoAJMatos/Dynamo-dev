//
// Created by joaoa on 21/04/2022.
//

#include "logger.h"

void logger(const std::string& text)
{
    std::cout << "[" << Time::getCurrentDateTime() << "][+] " << text << std::endl;
}