#ifndef X86_H
#define X86_H

#include "types.h"

struct multibootInfo {
    uint32_t flags;
    uint32_t memoryLow;
    uint32_t memoryHigh;
    uint32_t bootDevice;
    uint32_t commandLine;
    uint32_t moduleCount;
    uint32_t moduleAddresses;
    uint32_t syms0;
    uint32_t syms1;
    uint32_t syms2;
    uint32_t memoryMapLength;
    uint32_t memoryMapAddress;
    uint32_t drivesLength;
    uint32_t drivesAddress;
    uint32_t configTable;
    uint32_t bootloaderName;
    uint32_t apmTable;
    uint32_t vbeControlInformation;
    uint32_t vbeModeInformation;
    uint16_t vbeMode;
    uint32_t vbeInterfaceAddress;
    uint16_t vbeInterfaceLength;
};

#ifdef _KERNEL_
void *operator new (long unsigned int size);
void *operator new[] (long unsigned int size);
void operator delete (void *p);
void operator delete[] (void *p);
#endif

struct Stack {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t interruptVector, eCode;
    uint32_t eip, cs, eFlags, userESP, ss;
};

class Platform {
public:
    Platform();
    virtual ~Platform();

    virtual void EnableInterrupts();
    virtual void DisableInterrupts();
    virtual uint8_t Inport(uint8_t port);
    virtual void Outport(uint8_t port, uint8_t value);
    virtual void Reboot();
    virtual Stack GetStack();
};

class x86 : public Platform {
public:
    x86();
    ~x86();

    void EnableInterrupts();
    void DisableInterrupts();
    uint8_t Inport(uint8_t port);
    void Outport(uint8_t port, uint8_t value);
    void Reboot();
    Stack GetStack();
protected:
    uint64_t GetEAX();
    uint64_t GetEBX();
    uint64_t GetECX();
    uint64_t GetEDX();
    uint32_t GetCS();
    uint32_t GetDS();
    uint32_t GetES();
    uint32_t GetFS();
    uint32_t GetGS();
    uint32_t GetSS();
    uint64_t GetEDI();
    uint64_t GetESI();
    uint64_t GetEBP();
    uint64_t GetESP();
    uint32_t ReadCR0();
    uint32_t ReadCR3();
    uint32_t WriteCR0();
    uint32_t WriteCR3();
};

#endif
