#ifndef	INTERSECT_C
#define INTERSECT_C

#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "vec3f.h"
#include "Ray.h"

using namespace Vectors;
using namespace Rays;
using namespace std;

class Intersect{
    
    public:
        bool isHit;
        float t;
        Vec3f surfaceNormal;
        Vec3f intersectPoint;
        Vec3f vec1;
        Vec3f vec2;
        Vec3f vec3;

        Intersect(){
            isHit = false;
            t = -1;
        }
        Intersect(bool hit){
            isHit = false;
        }
        Intersect(bool isHit, float t, Vec3f intersectPoint, Vec3f surfaceNormal){
            this->isHit = isHit;
            this->t = t;
            this->surfaceNormal = surfaceNormal;
            this->intersectPoint = intersectPoint;
        }


};



#endif