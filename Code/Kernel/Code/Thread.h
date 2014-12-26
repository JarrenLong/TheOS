#ifndef _XPThreads_H_
#define _XPThreads_H_

#include "types.h"
#include "Core.h"

class Thread {
public:
    Thread(Callback function, uint64_t timeout = 3000) : funct(function), runTime(timeout) {}
    ~Thread() {
        Stop();
    }
    void SetThreadProc(Callback function);
    uint64_t GetID();
    bool Run();
private:
    void Stop();
    Callback funct;
    uint64_t runTime;
    uint64_t threadID;
};

#endif
