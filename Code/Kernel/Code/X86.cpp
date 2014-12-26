#include "X86.h"

#ifdef _KERNEL_
void *operator new(long unsigned int size) {
    return (void *)0;
}

void *operator new[](long unsigned int size) {
    return (void *)0;
}

void operator delete(void *p) {
}

void operator delete[](void *p) {
}
#endif

void x86::EnableInterrupts() {
    asm volatile("cli");
}

void x86::DisableInterrupts() {
    asm volatile("sti");
}

uint8_t x86::Inport(uint8_t port) {
    uint8_t ret;
asm volatile("inb %w1,%b0" : "=a"(ret) : "d"(port));
    return ret;
}

void x86::Outport(uint8_t port, uint8_t value) {
asm volatile("outb %b0,%w1" : : "a"(value), "d"(port));
}

void x86::Reboot() {
    int temp;
    do {
        temp = Inport( 0x64 );
        if ( temp & 1 )
            Inport( 0x60 );
    } while ( temp & 2 );
    Outport(0x64, 0xFE);
}

Stack x86::GetStack() {
    Stack regs;
    regs.eax = GetEAX();
    regs.ebx = GetEBX();
    regs.ecx = GetECX();
    regs.edx = GetEDX();
    regs.edi = GetEDI();
    regs.esi = GetESI();
    regs.ebp = GetEBP();
    regs.esp = GetESP();
    regs.cs = GetCS();
    regs.ds = GetDS();
    regs.ss = GetSS();
    return regs;
}

uint64_t x86::GetEAX() {
    uint64_t val = 0;
    //asm volatile("ret":"=a"(val): );
    return val;
}

uint64_t x86::GetEBX() {
    uint64_t val = 0;
    //asm volatile("mov %%ebx,%%eax;\nret;\n":"=a"(val):);
    return val;
}

uint64_t x86::GetECX() {
    uint64_t val = 0;
    //asm volatile("mov %%ecx,%%eax;\nret;\n":"=a"(val):);
    return val;
}

uint64_t x86::GetEDX() {
    uint64_t val = 0;
    //asm volatile("mov %%edx,%%eax;\nret;\n":"=a"(val):);
    return val;
}

uint32_t x86::GetCS() {
    uint32_t val = 0;
    asm volatile(
        "mov %%cs,%%ax;\n"
        "ret;\n"
    :"=a"(val)
                :
            );
    return val;
}

uint32_t x86::GetDS() {
    uint32_t val = 0;
    asm volatile(
        "mov %%ds,%%ax;\n"
        "ret;\n"
    :"=a"(val)
                :
            );
    return val;
}

uint32_t x86::GetES() {
    uint32_t val = 0;
    asm volatile(
        "mov %%es,%%ax;\n"
        "ret;\n"
    :"=a"(val)
                :
            );
    return val;
}

uint32_t x86::GetFS() {
    uint32_t val = 0;
    asm volatile(
        "mov %%fs,%%ax;\n"
        "ret;\n"
    :"=a"(val)
                :
            );
    return val;
}

uint32_t x86::GetGS() {
    uint32_t val = 0;
    asm volatile(
        "mov %%gs,%%ax;\n"
        "ret;\n"
    :"=a"(val)
                :
            );
    return val;
}

uint32_t x86::GetSS() {
    uint32_t val = 0;
    asm volatile(
        "mov %%ss,%%ax;\n"
        "ret;\n"
    :"=a"(val)
                :
            );
    return val;
}

uint64_t x86::GetEDI() {
    uint64_t val = 0;
    //asm volatile("mov %%edi,%%eax;\nret;\n":"=a"(val):);
    return val;
}

uint64_t x86::GetESI() {
    uint64_t val = 0;
    //asm volatile("mov %%esi,%%eax;\nret;\n":"=a"(val):);
    return val;
}

uint64_t x86::GetEBP() {
    uint64_t val = 0;
    //asm volatile("mov %%ebp,%%eax;\nret;\n":"=a"(val):);
    return val;
}

uint64_t x86::GetESP() {
    uint64_t val = 0;
    //asm volatile("mov %%esp,%%eax;\nret;\n":"=a"(val):);
    return val;
}

uint32_t x86::ReadCR0() {
    //asm volatile("mov eax, cr0\nretn");
}

uint32_t x86::ReadCR3() {
    //asm volatile("mov eax, cr3\nretn");
}

uint32_t x86::WriteCR0() {
    /*
    asm volatile("push ebp\n"
                 "mov ebp, esp\n"
                 "mov eax, [ebp+8]\n"
                 "mov cr0,  eax\n"
                 "pop ebp\n"
                 "retn");
                 */
}

uint32_t x86::WriteCR3() {
    /*
    asm volatile("push ebp\n"
                 "mov ebp, esp\n"
                 "mov eax, [ebp+8]\n"
                 "mov cr3, eax\n"
                 "pop ebp\n"
                 "retn");
                 */
}
