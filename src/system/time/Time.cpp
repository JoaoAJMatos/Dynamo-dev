//
// Created by Matos on 4/3/2022.
//

#include "Time.h"

/* CONSTRUCTOR */
Time::Time() {
    timeElapsed = std::chrono::duration<double>::zero();
}

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

void Time::sleep(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

/* GETTERS */
std::chrono::duration<double>::rep Time::getTimeElapsed() {
    return timeElapsed.count();
}

std::time_t Time::getTimestamp() {
    return std::time(nullptr);
}

std::string Time::getCurrentDateTime() {
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::string s(20, '\0');
    std::strftime(&s[0], s.size(), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return s;
}

