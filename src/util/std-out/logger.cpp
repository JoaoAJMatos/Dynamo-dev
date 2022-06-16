//
// Created by joaoa on 21/04/2022.
//

#include "logger.h"

void logger(const std::string& text)
{
    std::cout << "[" << Time::getCurrentDateTime() << "][+] " << text << std::endl;
}

void gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}