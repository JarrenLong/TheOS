#include "../Util/String.h"

class Return {
public:
    Return() : retCode(0) {}
    Return(long rc, String rs) {
        retCode = rc;
        retStr = rs;
    }
    ~Return() {}
    inline uint64_t GetLong() {
        return retCode;
    }
    String GetStr() {
        return retStr;
    }
    void SetRet(uint64_t code) {
        retCode = code;
    }
    void SetRet(String str) {
        retStr = str;
    }
private:
    uint64_t retCode;
    String retStr;
};
