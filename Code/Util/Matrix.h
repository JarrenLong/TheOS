#ifndef MYOS_MATRIX_H
#define MYOS_MATRIX_H

#include <math.h>
#include "Vector.h"

/*
XRot: (1,0,0,0),(0,cos(xrot),-sin(xrot),0),(0,sin(xrot),cos(xrot),0),(0,0,0,1)
YRot: (cos(yrot),0,sin(yrot),0),(0,1,0,0),(-sin(yrot),0,cos(yrot),0),(0,0,0,1)
ZRot: (cos(zrot),-sin(zrot),0,0),(sin(zrot),cos(zrot),0,0),(0,0,1,0),(0,0,0,1)
*/

class Matrix {
public:
    Matrix();
    ~Matrix();
    void Reset();
    void SetIdentity();
    void SetRotation(Matrix *rot);
    void SetTranslation(Vector t);
    void SetScale(Vector s);
    //Overload Operators
    Matrix *operator + (Matrix*);
    Matrix *operator - (Matrix*);
    Matrix *operator * (Matrix*);
    Vector *a,*b,*c,*d;
};

#endif //MYOS_MATRIX_H
