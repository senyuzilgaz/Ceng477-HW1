#ifndef CLASS_VEC
#define CLASS_VEC
#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include "parser.h"

using namespace std;
namespace Vectors{

class Vec3f{
    public:
        float x, y, z;
        Vec3f(){
            x=0;
            y=0;
            z=0;
        }        
        Vec3f(float a, float b, float c){
            x=a;
            y=b;
            z=c;
        }
        /*
        Vec3f(parser::Vec3f vec3f){
            x = vec3f.x;
            y = vec3f.y;
            z = vec3f.z;
        }*/
        Vec3f operator+(Vec3f a){
            Vec3f result;
            result.x = x + a.x;
            result.y = y + a.y;
            result.z = z + a.z;
            return result;
        }
        Vec3f operator=(Vec3f a){
            x = a.x;
            y = a.y;
            z = a.z;

            return *this;
        }
        // Vec3f operator=(parser::Vec3i a){
        //     x = a.x;
        //     y = a.y;
        //     z = a.z;

        //     return *this;
        // }
        Vec3f operator-(Vec3f a){
            Vec3f result;
            result.x = x - a.x;
            result.y = y - a.y;
            result.z = z - a.z;
            return result;
        }        
        float operator*(Vec3f a){
            float result = x * a.x + y * a.y + z * a.z;
            return result;
        }
        Vec3f operator*(double a){
            Vec3f result;
            result.x = x * a;
            result.y = y * a;
            result.z = z * a;
            return result;
        }
        Vec3f operator*(int a){
            Vec3f result;
            result.x = x * a;
            result.y = y * a;
            result.z = z * a;
            return result;
        }
        Vec3f operator*(float a){
            Vec3f result;
            result.x = x * a;
            result.y = y * a;
            result.z = z * a;
            return result;
        }
        Vec3f normalize(){
            return *this * (1.0/sqrt(*this * *this));
        }

        Vec3f cross(Vec3f t){
            float a = y * t.z - z * t.y;
            float b = z * t.x - x * t.z;
            float c = x * t.y - y * t.x;   

            return Vec3f(a, b, c);
        }
        Vec3f scalar(Vec3f a){
            return(Vec3f(x*a.x, y*a.y, z*a.z));
        }

        friend ostream& operator<<(ostream& os, Vec3f &x)
        {
            os << x.x << "   " << x.y << "   " << x.z << endl;
            return os;
        }
};
}


#endif