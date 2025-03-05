#include "Clock.h"
#include <cmath>

Clock::Clock() : hours(0), minutes(0), seconds(0) {
    update();
}

Clock::~Clock() {}

void Clock::update() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    hours = ltm->tm_hour;
    minutes = ltm->tm_min;
    seconds = ltm->tm_sec;
}

float Clock::getHourAngle() const {
    float hour = hours % 12;
    float angle = (hour + minutes / 60.0f) * 30.0f - 90.0f;
    return angle;
}

float Clock::getMinuteAngle() const {
    float angle = minutes * 6.0f - 90.0f;
    return angle;
}

float Clock::getSecondAngle() const {
    float angle = seconds * 6.0f - 90.0f;
    return angle;
}