#include "Quaternion.h"

Quaternion::Quaternion() {
    Reset();
}

Quaternion::~Quaternion() {
    Reset();
}

void Quaternion::Reset() {
    q = Vector(0,0,0,0);
    qt=q;
}

Vector Quaternion::Conjugate() {
    qt.setW(q.getW());
    qt.setX(-q.getX());
    qt.setY(-q.getY());
    qt.setZ(-q.getZ());
    return qt;
}

void Quaternion::Normalize() {
    q.Normalize();
}

Vector Quaternion::Inverse() {
    Normalize();
    return Conjugate();
}

float Quaternion::Length() {
    return q.Length();
}

void Quaternion::operator *= (Vector r)  {
    q.setW(q.getW()*r.getW() - q.getX()*r.getX() - q.getY()*r.getY() - q.getZ()*r.getZ());
    q.setX(q.getW()*r.getX() + q.getX()*r.getW() + q.getY()*r.getZ() - q.getZ()*r.getY());
    q.setY(q.getW()*r.getY() + q.getY()*r.getW() + q.getZ()*r.getX() - q.getX()*r.getZ());
    q.setZ(q.getW()*r.getZ() + q.getZ()*r.getW() + q.getX()*r.getY() - q.getY()*r.getX());
}
