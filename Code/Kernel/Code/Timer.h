#ifndef TIMER_H
#define TIMER_H

#include "Device.h"

class Timer : public Device {
public:
    Timer(uint64_t time);
    ~Timer();
    void SetDelay(uint64_t time);

    bool Start();
    void Pause();
private:
    uint64_t Handler();

    uint64_t Counter;
    uint64_t Delay;
    bool isRunning;
};

#endif
