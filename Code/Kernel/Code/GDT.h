#ifndef GDT_H
#define GDT_H

#include "Device.h"

typedef struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

typedef struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

class GDT : public Device {
public:
    GDT() {
        gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
        gp.base = (uint32_t)&gdt;
        //NULL
        SetGate(0, 0, 0, 0, 0);
        //32Bit Code Segment
        SetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
        //32Bit Data Segment
        SetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
        /*
        asm volatile(
            "lgdt [gp]\n"
            "mov ax, 0x10\n"
            "mov ds, ax\n"
            "mov es, ax\n"
            "mov fs, ax\n"
            "mov gs, ax\n"
            "mov ss, ax\n"
            "jmp 0x08:flush2\n"
            "flush2:\n"
            "ret"
        );
        */
    }
private:
    gdt_entry gdt[3];
    gdt_ptr gp;

    void SetGate(uint32_t num, uint64_t base, uint64_t limit, uint8_t access, int8_t gran) {
        gdt[num].base_low = (base & 0xFFFF);
        gdt[num].base_middle = (base >> 16) & 0xFF;
        gdt[num].base_high = (base >> 24) & 0xFF;
        gdt[num].limit_low = (limit & 0xFFFF);
        gdt[num].granularity = ((limit >> 16) & 0x0F);
        gdt[num].granularity |= (gran & 0xF0);
        gdt[num].access = access;
    }
};

#endif // GDT_H
