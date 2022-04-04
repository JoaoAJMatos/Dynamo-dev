#include <iostream>
#include "src/system/time/Time.h"



int main()
{
    Time t;

    t.startHighResClock();

    //Time::sleep(2000);

    t.stopHighResClock();

    std::cout << Time::getTimestamp();

    return 0;
}
