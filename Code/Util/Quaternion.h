#include "Vector.h"

class Quaternion {
public:
    Quaternion();
    ~Quaternion();
    void Reset();
    Vector Conjugate();
    void Normalize();
    Vector Inverse();
    float Length();
    void operator *= (Vector r);
private:
    Vector q, qt;
};
