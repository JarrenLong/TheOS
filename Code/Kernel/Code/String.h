#ifndef STRING_H
#define STRING_H

#include "types.h"

class String {
public:
    String();
    ~String();

    uint32_t Length();
    uint8_t *cStr();
    uint8_t Compare(const uint8_t *src);
    void Copy(const uint8_t *src);
    void Concat(const uint8_t *src);
    uint32_t Resize(uint32_t length);
private:
    uint8_t *str;
    uint32_t length;
};

#endif // STRING_H
