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
        double x, y, z;
        Vec3f(){
            x=0;
            y=0;
            z=0;
        }        
        Vec3f(double a, double b, double c){
            x=a;
            y=b;
            z=c;
        }
        
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
        Vec3f operator/(double a){
            Vec3f result;
            result.x = x / a;
            result.y = y / a;
            result.z = z / a;

            return result;
        }
        Vec3f operator-(Vec3f a){
            Vec3f result;
            result.x = x - a.x;
            result.y = y - a.y;
            result.z = z - a.z;
            return result;
        }        
        double operator*(Vec3f a){
            double result = x * a.x + y * a.y + z * a.z;
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
        Vec3f normalize(){
            return *this / sqrt(*this * *this);
        }

        Vec3f cross(Vec3f t){
            double a = y * t.z - z * t.y;
            double b = z * t.x - x * t.z;
            double c = x * t.y - y * t.x;   

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