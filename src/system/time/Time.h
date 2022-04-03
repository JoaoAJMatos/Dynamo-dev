//
// Created by Matos on 4/3/2022.
//
// The Time class has a set of timing utilities to be used in the program.

#ifndef DEV_DYNAMO_TIME_H
#define DEV_DYNAMO_TIME_H

#include <chrono>
#include <ctime>
#include <thread>

class Time {
private:
    /* MEMBER VARIABLES */
    std::chrono::duration<double> timeElapsed;
    std::chrono::high_resolution_clock::time_point highResTimePoint1;
    std::chrono::high_resolution_clock::time_point highResTimePoint2;


public:
    /* CONSTRUCTOR */
    Time();

    /* PUBLIC FUNCTIONS */
    void startHighResClock();
    void stopHighResClock();
    static void sleep(int ms);

    /* GETTERS */
    std::chrono::duration<double>::rep getTimeElapsed();
    static std::time_t getTimestamp();
};


#endif //DEV_DYNAMO_TIME_H
