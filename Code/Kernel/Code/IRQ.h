#ifndef IRQ_H
#define IRQ_H

#include "Device.h"

class IRQ : public Device {
public:
    IRQ() {
        /* Remap IRQ's #0-7 from enttries 8-15 to 32-47
        outportb(0x20, 0x11);
        outportb(0xA0, 0x11);
        outportb(0x21, 0x20);
        outportb(0xA1, 0x28);
        outportb(0x21, 0x04);
        outportb(0xA1, 0x02);
        outportb(0x21, 0x01);
        outportb(0xA1, 0x01);
        outportb(0x21, 0x0);
        outportb(0xA1, 0x0);
        */
        //idt_set_gate(32, (unsigned)irq0, 0x08, 0x8E);
        //Add the rest
        //idt_set_gate(47, (unsigned)irq15, 0x08, 0x8E);
        //Enable interrupts
        //asm volatile("sti");
    }
private:
    /*
        uint64_t Handler() {
            void (*handler)(struct regs *r);
            handler = irq_routines[r->int_no - 32];
            if (handler) {
                handler(r);
            }
            if (r->int_no >= 40) {
                outportb(0xA0, 0x20);
            }
            outportb(0x20, 0x20);
            return 0;
        }
    */
};

#endif //IRQ_H
