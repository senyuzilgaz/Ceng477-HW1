#ifndef CLASS_RAY
#define CLASS_RAY

#include <string>
#include <iostream>
#include <vector>
#include<math.h>
#include "parser.h"
#include "vec3f.h"

using namespace std;  
using namespace Vectors;
namespace Rays{

class Ray{
    private:
        float d, left, right, bottom, top;
        int width, height;
        Vec3f gaze, up, cameraPos;
        Vec3f m, q, s, u, v, w;
        parser::Camera camera;
    public:
        Vectors::Vec3f origin, direction;
            
        Ray(parser::Camera &camera){
            origin = Vec3f(0, 0, 0);
            direction = Vec3f(0, 0, 0);
            d = camera.near_distance;
            left = camera.near_plane.x;
            right = camera.near_plane.y;
            bottom = camera.near_plane.z;
            top = camera.near_plane.w;
            width = camera.image_width;
            height = camera.image_height;
            gaze = Vec3f(camera.gaze);
            up = Vec3f(camera.up);
            cameraPos = Vec3f(camera.position);

            w = (gaze * -1).normalize();
            v = up.normalize();
            u = v.cross(w).normalize();
            m = cameraPos + (gaze * d);
            q = m + (u * left) + (v * top);
        }
        Ray generateRay(int i, int j, parser::Camera &camera){
            float su = (i + 0.5) * (right - left) / width;
            float sv = (j + 0.5) * (top - bottom) / height;
            //For loopun öncesine al her seferinde değiştirmene gerek yok.
            s = q + (u * su) + (v * -sv);

            origin = cameraPos;
            direction = s - cameraPos;

            return *this;
        }
};

}
#endif