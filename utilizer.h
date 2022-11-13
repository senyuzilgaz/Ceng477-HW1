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

	int colorClamp(float color){
		if(color > 255)
			return 255;
		if(color < 0)
			return 0;
		return round(color + 0.5);
	}
	
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
		Intersect info = Intersect(true, t, ip, sn);

		return info;
	}
	Vec3f calculateColor(Ray &ray, parser::Scene &scene, int lightIndex){
		int material, minI= -1;
		Vec3f c, intersectionPoint, surfaceNormal, irradiance, diffuse, line, normal;
		float t, minT = 90000; // some large number
		//Intersect with spheres
		for (int sphereIndex=0; sphereIndex<scene.spheres.size(); sphereIndex++)
		{
			Intersect info = intersectSphere(ray, scene.spheres[sphereIndex], scene.vertex_data);
			if (info.isHit && info.t<minT && info.t>=0)
			{
				// can be replaced with any material property
				cout << info.isHit;
				minI = sphereIndex;
				minT = info.t;
				intersectionPoint = info.intersectPoint;
				surfaceNormal = info.surfaceNormal;
			}
		}

		if (minI!=-1)
		{
			material = scene.spheres[minI].material_id -1; //Ambient
			c = scene.materials[material].ambient;
			c = c.scalar(scene.ambient_light);


			line = scene.point_lights[lightIndex].position - intersectionPoint;
			irradiance =  scene.point_lights[lightIndex].intensity / ( line * line);
			normal = line.normalize();
			float dot = normal * surfaceNormal;
			if (dot < 0) dot = 0;
			c = c + (scene.materials[material].diffuse * dot).scalar(irradiance);

		}
		else{
			c.x = (float)scene.background_color.x;
			c.y = (float)scene.background_color.y;
			c.z = (float)scene.background_color.z;
		}
		return c;	
	}		
		

		
	
}

#endif