#ifndef CLOCK_H
#define CLOCK_H

#include <SDL2/SDL.h>
#include <ctime>

class Clock {
public:
    Clock();
    ~Clock();

    void update();
    float getHourAngle() const;
    float getMinuteAngle() const;
    float getSecondAngle() const;

private:
    int hours;
    int minutes;
    int seconds;
};

#endif // CLOCK_H