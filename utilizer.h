#ifndef	UTIL_G
#define UTIL_G

#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "vec3f.h"
#include "Ray.h"

using namespace Vectors;
using namespace Rays;
using namespace std;

namespace utilizer{

	float intersectSphere(Ray ray, parser::Sphere sphere, vector<Vec3f> vertexData){
		float A,B,C; //constants for the quadratic equation
		
		float delta;
		
		Vec3f c;
		
		c = vertexData[sphere.center_vertex_id];
		
		float t,t1,t2;
		
		C = (ray.origin.x-c.x)*(ray.origin.x-c.x)+(ray.origin.y-c.y)*(ray.origin.y-c.y)+(ray.origin.z-c.z)*(ray.origin.z-c.z)-sphere.radius*sphere.radius;

		B = 2*ray.direction.x*(ray.origin.x-c.x)+2*ray.direction.y*(ray.origin.y-c.y)+2*ray.direction.z*(ray.origin.z-c.z);
		
		A = ray.direction.x*ray.direction.x+ray.direction.y*ray.direction.y+ray.direction.z*ray.direction.z;
		
		delta = B*B-4*A*C;
		
		if (delta<0) return -1;
		else if (delta==0)
		{
			t = -B / (2*A);
		}
		else
		{
			delta = sqrt(delta);
			A = 2 * A;
			t1 = (-B + delta) / A;
			
			// t2 = (-B - delta) / A;
			t = t1;
			// t1 is actually always less than t2
			// if (t1 < t2) t = t1; else t = t2;
		}
		
		return t;
	}
	Vec3f calculateColor(Ray &ray, parser::Scene &scene){
		int i, minI = -1;
		Vec3f c, L, N, P;
		float t, minT = 90000; // some large number
		for (i=0; i<scene.spheres.size(); i++)
		{
			t = intersectSphere(ray, scene.spheres[i], scene.vertex_data);
			if (t<minT && t>=0)
			{
				int material = scene.spheres[i].material_id;
				c = scene.materials[material].ambient ; // can be replaced with any material property
				minI = i;
				minT = t;
			}
		}

		if (minI!=-1)
		{

			P = ray.origin + (ray.direction * minT);
			L = scene.ambient_light - P;
			N = P - scene.vertex_data[scene.spheres[minI].center_vertex_id];
			L = L.normalize();
			N = N.normalize();
			c = c * (L * N);
		}
		else{
			
			c.x = (float)scene.ambient_light.x;
			c.y = (float)scene.ambient_light.y;
			c.z = (float)scene.ambient_light.z;
		}
		return c;
	}
}

#endif