#include <math.h>

#define VECTOR_TOLERANCE 0.00000001

class Vector {
public:
    Vector();
    Vector(float X, float Y);
    Vector(float X, float Y, float Z);
    Vector(float X, float Y, float Z, float W);
    virtual ~Vector();
    Vector getVector();
    void Reset() {x=y=z=w=0;}
    //Get/Set values
    float getW();
    float getX();
    float getY();
    float getZ();
    void setW(float W);
    void setX(float X);
    void setY(float Y);
    void setZ(float Z);
    //Some handy Functions
    float Magnitude();
    float Length();
    void Normalize();
    float Dot(Vector a, Vector b);
    Vector XProd(Vector a, Vector b);
    //Overloaded Operators
    Vector operator  + (float a);
    Vector operator  - (float a);
    Vector operator  * (float a);
    Vector operator  / (float a);
    Vector &operator += (float a);
    Vector &operator -= (float a);
    Vector &operator *= (float a);
    Vector &operator /= (float a);
    Vector operator + (Vector &a);
    Vector operator - (Vector &a);
    Vector operator * (Vector &a);
    Vector operator / (Vector &a);
    Vector &operator += (Vector &a);
    Vector &operator -= (Vector &a);
    Vector &operator *= (Vector &a);
    Vector &operator /= (Vector &a);
    bool operator == (Vector &a);
    bool operator != (Vector &a);
private:
    float x, y, z, w;
};
