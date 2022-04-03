#include <iostream>
#include "src/system/time/Time.h"



int main()
{
    Time t;

    t.startHighResClock();

    for (int i=0; i<1000; ++i) std::cout << "*";

    t.stopHighResClock();

    std::cout << t.getTimeElapsed();

    return 0;
}
