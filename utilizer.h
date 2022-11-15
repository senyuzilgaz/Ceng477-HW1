#ifndef	UTIL_G
#define UTIL_G

#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "vec3f.h"
#include "Ray.h"
#include "intersect.h"

using namespace Vectors;
using namespace Rays;
using namespace std;

int colorClamp(double &color);
double determinant(Vec3f a, Vec3f b, Vec3f c);
Vec3f calculateAmbiance(parser::Material& material, Vec3f& ambientLight);
Vec3f calculateDiffuse(parser::Material &material, parser::PointLight &pointLight, Vec3f& intersectionPoint, Vec3f& surfaceNormal);
Vec3f calculateSpecular(parser::Material& material, parser::PointLight &pointLight, Vec3f& intersectionPoint, Vec3f& surfaceNormal, Vec3f& rayDirection);
Vec3f calculateReflection(parser::Material& material, parser::Scene &scene, parser::Camera &camera, Vec3f& intersectionPoint, Vec3f& surfaceNormal, Vec3f& rayDirection,  int recursionDepth);
Intersect intersectSphere(Ray& ray, parser::Sphere &sphere, vector<Vec3f>& vertexData);
Intersect intersectTriangle(Ray &ray, parser::Face& face, vector<Vec3f>& vertexData);
bool findIntersection(Ray &ray, vector<Vec3f> &vertexData, parser::Scene &scene, Vec3f &intersectionPoint, Vec3f &surfaceNormal, int &materialId, double &minT );
Vec3f calculateColor(Ray &ray, parser::Scene &scene, parser::Camera &camera, int recursionDepth);




#endif