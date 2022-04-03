//
// Created by Matos on 4/3/2022.
//

#include "Time.h"

/* PUBLIC FUNCTIONS */
void Time::startHighResClock() {
    // Get current time
    highResTimePoint1 = std::chrono::high_resolution_clock::now();
}

void Time::stopHighResClock() {
    // Get current time
    highResTimePoint2 = std::chrono::high_resolution_clock::now();

    // Calculate de duration and store it in the timeElapsed variable
    timeElapsed = std::chrono::duration_cast<std::chrono::duration<double>>(highResTimePoint2 - highResTimePoint1);
}

/* GETTERS */
std::chrono::duration<double>::rep Time::getTimeElapsed() {
    return timeElapsed.count();
}

std::time_t Time::getTimestamp() {
    return std::time(nullptr);
}
