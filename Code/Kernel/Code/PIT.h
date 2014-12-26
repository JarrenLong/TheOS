#ifndef PIT_H
#define PIT_H

#include "Device.h"

class PIT {
public:
    PIT() {
        ticks = 0;
        //irq_install_handler(0, timer_handler);
    }
    ~PIT() {
        ticks = 0;
    }
protected:
    void SetPhase(uint32_t hz) {
        uint32_t divisor = 1193180 / hz;       /* Calculate our divisor */
        outportb(0x43, 0x36);             /* Set our command byte 0x36 */
        outportb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
        outportb(0x40, divisor >> 8);     /* Set high byte of divisor */
    }
    void Sleep(uint32_t ms) {
        uint64_t eticks;

        eticks = ticks + ms;
        while (ticks < eticks);
    }
private:
    uint64_t ticks;

    uint64_t Handler() {
        ticks++;
        if (ticks % 18 == 0) {
            //puts("One second has passed\n");
        }
    }
};

#endif //PIT_H
