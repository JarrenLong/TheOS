#include "Timer.h"

Timer::Timer(uint64_t time) {
    SetDelay(time);
    isRunning = false;
}

Timer::~Timer() {
}

void Timer::SetDelay(uint64_t time) {
    Delay = time;
    Counter = 0;
}

uint64_t Timer::Handler() {
    return 1;
}

bool Timer::Start() {
    isRunning = true;
    do {
        //Get time
        Counter++;
        if (Counter > Delay) {
            return Handler();
        }
    } while (isRunning);
    return false;
}

void Timer::Pause() {
    isRunning = false;
}
