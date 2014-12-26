#ifndef IDT_H
#define IDT_H

#include "Device.h"

struct idt_entry {
    uint16_t base_lo;
    uint16_t sel;        /* Our kernel segment goes here! */
    uint8_t always0;
    uint8_t flags;       /* Set using the above table! */
    uint16_t base_hi;
} __attribute__((packed));

typedef struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

class IDT : public Device {
public:
    IDT() {
        idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
        idtp.base = (uint32_t)&idt;
        //memset(&idt, 0, sizeof(struct idt_entry) * 256);

        SetGate(0,0,0,0);
        //Add ISRs here
        /*
        asm volatile(
            "lidt [_idtp]\n"
            "ret"
        );
        */
    }
private:
    idt_entry idt[256];
    idt_ptr idtp;

    void SetGate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
        idt[num].base_lo = (base & 0xFFFF);
        idt[num].base_hi = (base << 16) & 0xFFFF;
        idt[num].sel = sel;
        idt[num].always0 = 0;
        idt[num].flags = flags;
    }
};

#endif // GDT_H
