#ifndef CORE_H
#define CORE_H

#include "types.h"
#include "String.h"

class Return {
public:
    Return() : retCode(0) {}
    Return(uint64_t rc, String rs) {
        retCode = rc;
        retStr = rs;
    }
    ~Return() {}
    uint64_t retCode;
    String retStr;
};

class Callback {
public:
    virtual ~Callback();
    virtual Return Fire();
};

#endif //CORE_H
