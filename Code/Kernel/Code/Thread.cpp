#include "Thread.h"

void Thread::SetThreadProc(Callback function) {
    funct = function;
}

uint64_t Thread::GetID() {
    return threadID;
}

bool Thread::Run() {
    /*
    Create thread
    Assign threadID and callback function
    Run callback function
    */
    return true;
}

void Thread::Stop() {
    /*
    Wait for the thread to stop running
    Terminate it
    Cleanup
    */
}

