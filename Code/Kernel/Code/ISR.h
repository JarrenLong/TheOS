#ifndef ISR_H
#define ISR_H

#include "IDT.h"
#include "Device.h"

static const char *exceptions[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved"
};

class ISR : public Device {
public:
    ISR(){}
    ISR(IDT interrupts) {
        ints = interrupts;
        for(int i=0;i<32;i++) {
            //ints.SetGate(i,Handler(i),0x08,0x8E);
        }
    }
private:
    IDT ints;
    uint64_t Handler(uint8_t code) {
        Stack regs = handle->GetStack();
        if (regs.interruptVector < 32) {
            //puts("Exception: ");
            //puts(exceptions[regs.interruptVector]);
            for (;;);
        }
        return 0;
    }
};

#endif //ISR_H
