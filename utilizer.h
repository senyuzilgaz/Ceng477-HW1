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

namespace utilizer{

	Intersect intersectSphere(Ray ray, parser::Sphere sphere, vector<Vec3f> vertexData){
		float A, B, C, delta, r; //constants for the quadratic equation		
		Vec3f c;
		
		c = vertexData[sphere.center_vertex_id -1];
		r = sphere.radius;

		float t,t1,t2;
		
		C = (ray.origin.x-c.x)*(ray.origin.x-c.x)+(ray.origin.y-c.y)*(ray.origin.y-c.y)+(ray.origin.z-c.z)*(ray.origin.z-c.z)-sphere.radius*sphere.radius;

		B = 2*ray.direction.x*(ray.origin.x-c.x)+2*ray.direction.y*(ray.origin.y-c.y)+2*ray.direction.z*(ray.origin.z-c.z);
		
		A = ray.direction.x*ray.direction.x+ray.direction.y*ray.direction.y+ray.direction.z*ray.direction.z;
		
		delta = B*B-4*A*C;
		
		if (delta<0){
			return Intersect(false);
		}	
		else if (delta==0)
		{
			t = -B / (2*A);
		}
		else
		{
			delta = sqrt(delta);
			A = 2 * A;
			t1 = (-B + delta) / A;
			
			t2 = (-B - delta) / A;
			// t1 is actually always less than t2
			if (t1 < t2) t = t1; else t = t2;
		}
		Vec3f ip = ray.origin + ray.direction*t;
		Vec3f sn = (ip - c) / r;
		Intersect info = new Intersect(true, t, ip, sn);

		return info;
	}
	Vec3f calculateColor(Ray &ray, parser::Scene &scene){
		int i, minI = -1;
		Vec3f c, ip, n;
		c.x = (float)scene.background_color.x;
		c.y = (float)scene.background_color.y;
		c.z = (float)scene.background_color.z;
		float t, minT = 90000; // some large number
		Vec3f irradiance;
		//Intersect with spheres
		for (i=0; i<scene.spheres.size(); i++)
		{
			Intersect info = intersectSphere(ray, scene.spheres[i], scene.vertex_data);
			t = info.t;
			if (t<minT && t>=0)
			{
				 // can be replaced with any material property
				minI = i;
				minT = t;
				ip = info.intersectPoint;
			}
		}

		if (minI!=-1)
		{
			int material = scene.spheres[minI].material_id -1;
			c = scene.materials[material].ambient;
			c = c.scalar(scene.ambient_light);
			Vec3f d = scene.point_lights[0].position - ip ;
			irradiance = scene.point_lights[0].intensity/ ( d * d);
			Vec3f dnormal = d.normalize();
			float dot = dnormal * d;
			if (dot < 0) dot = 0;
			c = c +(scene.materials[material].diffuse * dot).scalar(irradiance);

		}
		return c;
	}
}

#endif