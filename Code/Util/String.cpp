#include "String.h"

String::String() {
    str = new uint8_t[0];
    length = 0;
}

String::~String() {
    length = 0;
    delete [] str;
}

uint32_t String::Length() {
    return length;
}

uint8_t *String::cStr() {
    return (uint8_t*)str;
}

uint8_t String::Compare(const uint8_t *src) {
    uint32_t i = 0;
    while (str[i] != '\0' && src[i] != '\0') {
        if (str[i] != src[i]) {
            return 0;
        }
        i++;
    }
    return 1;
}

void String::Copy(const uint8_t *src) {
    uint32_t i=0;
    while (src[i] != '\0') {
        i++;
    }
    length = i;
    str = (uint8_t*)src;
}

void String::Concat(const uint8_t *src) {
    uint32_t i = 0, c = 0;
    //get length of input
    while (src[i] != '\0') {
        i++;
    }

    c = Resize(length + i);

    for (;i<length;i++) {
        str[i] = src[i-c];
    }
}

uint32_t String::Resize(uint32_t len) {
    uint8_t buf[length];
    uint32_t i=0;

    for(i=0;i<length;i++) {
        buf[i] = str[i];
    }

    str = new uint8_t[len];

    for(i=0;i<length;i++) {
        str[i] = buf[i];
    }
    for (i=length;i<len - length;i++) {
        str[i] = 'X';
    }
    length = len;
    return length;
}
