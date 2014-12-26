#include "Vector.h"

    Vector::Vector() {
        w=x=y=z=0;
    }

    Vector::Vector(float X, float Y) {
        w=z=0;
        x=X;
        y=Y;
    }

    Vector::Vector(float X, float Y, float Z) {
        w=0;
        x=X;
        y=Y;
        z=Z;
    }

    Vector::Vector(float X, float Y, float Z, float W) {
        w=W;
        x=X;
        y=Y;
        z=Z;
    }

    Vector::~Vector() {
        w=x=y=z=0;
    }

    Vector Vector::getVector() {
        return *this;
    }

    float Vector::getW() {
        return w;
    }

    float Vector::getX() {
        return x;
    }

    float Vector::getY() {
        return y;
    }

    float Vector::getZ() {
        return z;
    }

    void Vector::setW(float W) {
        w = W;
    }

    void Vector::setX(float X) {
        x = X;
    }

    void Vector::setY(float Y) {
        y = Y;
    }

    void Vector::setZ(float Z) {
        z = Z;
    }

    Vector Vector::operator  + (float a) {
        return Vector(x+a,y+a,z+a,w+a);
    }

    Vector Vector::operator  - (float a) {
        return Vector(x-a,y-a,z-a,w-a);
    }

    Vector Vector::operator  * (float a) {
        return Vector(x*a,y*a,z*a,w*a);
    }

    Vector Vector::operator  / (float a) {
        return Vector(x/a,y/a,z/a,w/a);
    }

    Vector& Vector::operator += (float a) {
        x+=a;
        y+=a;
        z+=a;
        w+=a;
        return *this;
    }

    Vector& Vector::operator -= (float a) {
        x-=a;
        y-=a;
        z-=a;
        w-=a;
        return *this;
    }

    Vector& Vector::operator *= (float a) {
        x*=a;
        y*=a;
        z*=a;
        w*=a;
        return *this;
    }

    Vector& Vector::operator /= (float a) {
        x/=a;
        y/=a;
        z/=a;
        w/=a;
        return *this;
    }

    Vector Vector::operator + (Vector &a) {
        return Vector(
                   x+a.getX(),
                   y+a.getY(),
                   z+a.getZ(),
                   w+a.getW()
               );
    }

    Vector Vector::operator - (Vector &a) {
        return Vector(
                   x-a.getX(),
                   y-a.getY(),
                   z-a.getZ(),
                   w-a.getW()
               );
    }

    Vector Vector::operator * (Vector &a) {
        return Vector(
                   x*a.getX(),
                   y*a.getY(),
                   z*a.getZ(),
                   w*a.getW()
               );
    }

    Vector Vector::operator / (Vector &a) {
        return Vector(
                   x/a.getX(),
                   y/a.getY(),
                   z/a.getZ(),
                   w/a.getW()
               );
    }

    Vector& Vector::operator += (Vector &a) {
        w += a.getW();
        x += a.getX();
        y += a.getY();
        z += a.getZ();
        return *this;
    }

    Vector& Vector::operator -= (Vector &a) {
        w -= a.getW();
        x -= a.getX();
        y -= a.getY();
        z -= a.getZ();
        return *this;
    }

    Vector& Vector::operator *= (Vector &a) {
        w *= a.getW();
        x *= a.getX();
        y *= a.getY();
        z *= a.getZ();
        return *this;
    }

    Vector& Vector::operator /= (Vector &a) {
        w /= a.getW();
        x /= a.getX();
        y /= a.getY();
        z /= a.getZ();
        return *this;
    }

    bool Vector::operator == (Vector &a) {
        if (
            w==a.getW() &&
            x==a.getX() &&
            y==a.getY() &&
            z==a.getZ()
        ) {
            return true;
        }
        return false;
    }

    bool Vector::operator != (Vector &a) {
        if (
            w!=a.getW() ||
            x!=a.getX() ||
            y!=a.getY() ||
            z!=a.getZ()
        ) {
            return true;
        }
        return false;
    }

    float Vector::Magnitude() {
        return w*w + x*x + y*y + z*z;;
    }

    float Vector::Length() {
        return (float)sqrt(Magnitude());
    }

    void Vector::Normalize() {
        if (fabs(Magnitude() - 1.0f) > VECTOR_TOLERANCE) {
            float mag = Length();
            w /= mag;
            x /= mag;
            y /= mag;
            z /= mag;
        }
    }

    float Vector::Dot(Vector a, Vector b) {
        return (
                   (a.getW() * b.getW()) +
                   (a.getX() * b.getX()) +
                   (a.getY() * b.getY()) +
                   (a.getZ() * b.getZ())
               );
    }

    Vector Vector::XProd(Vector a, Vector b) {
        Vector t(0,0,0,0);
        //t.setW();
        t.setX((a.getY() * b.getZ()) - (a.getZ() * b.getY()));
        t.setY((a.getZ() * b.getX()) - (a.getX() * b.getZ()));
        t.setZ((a.getX() * b.getY()) - (a.getY() * b.getX()));
        return t;
    }

//void Vector::Draw(Vector *to, Vector *from, Vector *color) {
    /*
    glColor3f(color->getX(), color->getY(), color->getZ());
    glBegin(GL_LINES);
    glVertex3f(from->getX(), from->getY(), from->getZ());
    glVertex3f(to->getX(), to->getY(), to->getZ());
    glEnd();
    glColor3f(1, 1, 1);
    glLineWidth(1);
    */
//}

/*
void ProjectOntoPlane(Vector *NormalToPlane, Vector *VecToProject) {
    Vector	Projection = Parallel(VecToProject, NormalToPlane);
    *VecToProject = *VecToProject - Projection;
}

Vector Perp(Vector *a, Vector *axis) {
    return (*a - Parallel(a, axis));
}

Vector Parallel(Vector *a, Vector *axis) {
    return (*axis * DotProduct(a, axis));
}

Vector	Mirror(Vector *a, Vector *axis) {
    return (*a - (Perp(a, axis)*2));
}

void RotateVector(float	Angle, Vector *VecToRotAbout, Vector *VecToRotate) {
    float cosTheta = (float)cos(Angle);
    float sinTheta = (float)sin(Angle);
    *VecToRotate=Perp(VecToRotate,VecToRotAbout)*cosTheta+(CrossProduct(*VecToRotAbout, *VecToRotate)*sinTheta)
                 + Parallel(VecToRotate, VecToRotAbout);
}
*/
