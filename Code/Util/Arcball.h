#ifndef _ARCBALL_H_
#define _ARCBALL_H_

#include <math.h>
#include "Quaternion.h"

class Arcball {
public:
    Arcball() {}

    Arcball(float w, float h) {
        SetBounds(w,h);
    }

    ~Arcball() {
        a.Reset();
        b.Reset();
        aw=ah=0;
    }

    void SetBounds(float x, float y) {
        if(x>1.0f && y>1.0f) {
            aw = 1.0f/((x-1.0f)*0.5f);
            ah = 1.0f/((y-1.0f)*0.5f);
        }
    }

    Vector drag(Quaternion rot) {
    	Vector t, pt;
	pt = map(b);
    	if (rot) {
	    Vector Perp;
	    Perp.XProd(a,b);
	    if (Perp.Length() > 0.0) {
		rot.q->x = Perp.x;
		rot.q->y = Perp.y;
		rot.q->z = Perp.z;
		rot.q->w = t.Dot(a,b);
            } else {

		rot.q->Reset();
	    }
	}

	return rot.q;
    }



    Vector map(Vector pt) {
	pt.x = (pt.x * aw) - 1.0f;
	pt.y = 1.0f - (pt.y * ah);

	float l = pt.Length();
	if (l > 1.0f) {
	    pt.Normalize();
	    v = pt;
	} else {
	    Vector v;
	    v.x = pt.x;
	    v.y = pt.y;
	    v.z = sqrt(1.0f - l);

	    v.w = 0;
	}
        return v;
    }

private:
    float aw,ah;
    Vector a,b;
};

#endif
