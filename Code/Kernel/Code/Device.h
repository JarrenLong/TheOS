#ifndef DEVICE_H
#define DEVICE_H

#include "X86.h"

class Device {
public:
    x86 *handle;

    Device() {}
    Device(uint64_t param) {}
    Device(x86 *ptr) : handle(ptr) {}
    virtual ~Device() {
        if(handle) {
            delete handle;
        }
    }

    virtual uint32_t Read() {return 0;}
    virtual uint32_t Write() {return 0;}
protected:
private:
    virtual uint64_t Handler() {return 0;}
};

#endif //DEVICE_H
