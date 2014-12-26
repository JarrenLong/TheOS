#include "Matrix.h"

Matrix::Matrix() {
    Reset();
}

Matrix::~Matrix() {
    Reset();
    delete a;
    delete b;
    delete c;
    delete d;
}

void Matrix::Reset() {
    a->Reset();
    b->Reset();
    c->Reset();
    d->Reset();
}

void Matrix::SetIdentity() {
    Reset();
    a->setX(1);
    b->setY(1);
    c->setZ(1);
    d->setW(1);
}

void Matrix::SetTranslation(Vector t) {
    SetIdentity();
    d->setX(t.getX());
    d->setY(t.getY());
    d->setZ(t.getZ());
}

void Matrix::SetScale(Vector s) {
    Reset();
    a->setX(s.getX());
    b->setY(s.getY());
    c->setZ(s.getZ());
    d->setW(1);
}

Matrix *Matrix::operator * (Matrix *i) {
    Matrix *t = new Matrix();
//    t->a.x = a.x*i->a.x +
    return t;
}

void Matrix::SetRotation(Matrix *rot) {
    /*
        GLfloat n, s;
        GLfloat xs, ys, zs;
        GLfloat wx, wy, wz;
        GLfloat xx, xy, xz;
        GLfloat yy, yz, zz;
        assert(NewObj && q1);
        n = (q1->s.X * q1->s.X) + (q1->s.Y * q1->s.Y) + (q1->s.Z * q1->s.Z) + (q1->s.W * q1->s.W);
        s = (n > 0.0f) ? (2.0f / n) : 0.0f;
        xs = q1->s.X * s;  ys = q1->s.Y * s;  zs = q1->s.Z * s;
        wx = q1->s.W * xs; wy = q1->s.W * ys; wz = q1->s.W * zs;
        xx = q1->s.X * xs; xy = q1->s.X * ys; xz = q1->s.X * zs;
        yy = q1->s.Y * ys; yz = q1->s.Y * zs; zz = q1->s.Z * zs;
        NewObj->s.XX = 1.0f - (yy + zz); NewObj->s.YX =         xy - wz;  NewObj->s.ZX =         xz + wy;
        NewObj->s.XY =         xy + wz;  NewObj->s.YY = 1.0f - (xx + zz); NewObj->s.ZY =         yz - wx;
        NewObj->s.XZ =         xz - wy;  NewObj->s.YZ =         yz + wx;  NewObj->s.ZZ = 1.0f - (xx + yy);
        */
}
/*
inline
static void Matrix3fMulMatrix3f(Matrix3fT* NewObj, const Matrix3fT* m1)
{
    Matrix3fT Result;
    assert(NewObj && m1);
    Result.s.M00 = (NewObj->s.M00 * m1->s.M00) + (NewObj->s.M01 * m1->s.M10) + (NewObj->s.M02 * m1->s.M20);
    Result.s.M01 = (NewObj->s.M00 * m1->s.M01) + (NewObj->s.M01 * m1->s.M11) + (NewObj->s.M02 * m1->s.M21);
    Result.s.M02 = (NewObj->s.M00 * m1->s.M02) + (NewObj->s.M01 * m1->s.M12) + (NewObj->s.M02 * m1->s.M22);
    Result.s.M10 = (NewObj->s.M10 * m1->s.M00) + (NewObj->s.M11 * m1->s.M10) + (NewObj->s.M12 * m1->s.M20);
    Result.s.M11 = (NewObj->s.M10 * m1->s.M01) + (NewObj->s.M11 * m1->s.M11) + (NewObj->s.M12 * m1->s.M21);
    Result.s.M12 = (NewObj->s.M10 * m1->s.M02) + (NewObj->s.M11 * m1->s.M12) + (NewObj->s.M12 * m1->s.M22);
    Result.s.M20 = (NewObj->s.M20 * m1->s.M00) + (NewObj->s.M21 * m1->s.M10) + (NewObj->s.M22 * m1->s.M20);
    Result.s.M21 = (NewObj->s.M20 * m1->s.M01) + (NewObj->s.M21 * m1->s.M11) + (NewObj->s.M22 * m1->s.M21);
    Result.s.M22 = (NewObj->s.M20 * m1->s.M02) + (NewObj->s.M21 * m1->s.M12) + (NewObj->s.M22 * m1->s.M22);
    *NewObj = Result;
}
*/
