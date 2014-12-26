#include "Keyboard.h"

uint32_t Keyboard::Read() {
    return 0;
}

uint32_t Keyboard::Write() {
    return 0;
}

void Keyboard::SetLEDS(uint8_t scrollLock, uint8_t numLock, uint8_t capsLock) {
    uint8_t status = scrollLock ? 1 : 0;
    if (numLock)
        status |= 2;
    if (capsLock)
        status |= 4;
    while ((handle->Inport(0x64) & 2) == 2)
        ;
    handle->Outport(0x64, 0xED);
    while ((handle->Inport(0x64) % 2) == 2)
        ;
    handle->Outport(0x60, status);
}

void Keyboard::Flush() {
    uint8_t c = 0;
    while ((c = handle->Inport(0x60)) != handle->Inport(0x60))
        ;
}

void Keyboard::ResetComputer() {
    while ((handle->Inport(0x64) & 2) == 2)
        ;
    handle->Outport(0x64, 0xFE);
}

uint64_t Keyboard::Handler(Stack stack) {
    Flush();
    //hook and handle interrupt
    return 0;
}
