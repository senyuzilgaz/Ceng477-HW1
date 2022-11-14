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

	int colorClamp(float &color){
		if(color > 255)
			return 255;
		if(color < 0)
			return 0;
		return round(color + 0.5);
	}

	float determinant(Vec3f a, Vec3f b, Vec3f c){
		float det = a.x * (b.y * c.z - c.y * b.z) 
		          + a.y * (c.x * b.z - b.x * c.z)
				  + a.z * (b.x * c.y - b.y * c.x);
		return det;
	}	

	Vec3f calculateAmbiance(parser::Material& material, Vec3f& ambientLight){
		return material.ambient.scalar(ambientLight);
	}
	Vec3f calculateDiffuse(parser::Material &material, parser::PointLight &pointLight, Vec3f& intersectionPoint, Vec3f& surfaceNormal){
			Vec3f line = pointLight.position - intersectionPoint; //Diffuse
			Vec3f irradiance = line * line != 0.0 ?  pointLight.intensity / ( line * line): Vec3f(0,0,0);
			Vec3f normal = line.normalize();
			float cosa = fmax(0, normal * surfaceNormal);
			return (material.diffuse * cosa).scalar(irradiance);
	}
	Vec3f calculateSpecular(parser::Material& material, parser::PointLight &pointLight, Vec3f& intersectionPoint, Vec3f& surfaceNormal, Vec3f& rayDirection){
			Vec3f wi, wo, h;
            Vec3f line = pointLight.position - intersectionPoint; 
            Vec3f irradiance = line * line != 0.0 ?  pointLight.intensity / ( line * line): Vec3f(0,0,0);
            wi = (pointLight.position - intersectionPoint);
            wi = wi.normalize();
            h = (wi - rayDirection).normalize();

            float cosb = fmax(0,h * surfaceNormal);
            return (material.specular * pow(cosb, material.phong_exponent)).scalar(irradiance);
	}

	
	Intersect intersectSphere(Ray& ray, parser::Sphere &sphere, vector<Vec3f>& vertexData){
		float A, B, C, delta, r; //constants for the quadratic equation				
		Vec3f c;
		
		c = vertexData[sphere.center_vertex_id -1];
		r = sphere.radius;

		float t, t1, t2;
		
		C = (ray.origin.x-c.x)*(ray.origin.x-c.x)+(ray.origin.y-c.y)*(ray.origin.y-c.y)+(ray.origin.z-c.z)*(ray.origin.z-c.z)-sphere.radius*sphere.radius;
		B = 2*ray.direction.x*(ray.origin.x-c.x)+2*ray.direction.y*(ray.origin.y-c.y)+2*ray.direction.z*(ray.origin.z-c.z);
		A = ray.direction.x*ray.direction.x+ray.direction.y*ray.direction.y+ray.direction.z*ray.direction.z;
		
		delta = B*B - 4*A * C;
		
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
			A = 2*A;
			t1 = (-B + delta) / A;
			
			t2 = (-B - delta) / A;
			// t1 is actually always less than t2
			if (t1 < t2) t = t1; else t = t2;
		}
		Vec3f intersectionPoint = ray.origin + ray.direction*t;
		Vec3f surfaceNormal = ((intersectionPoint - c)).normalize();
		Intersect info = Intersect(true, t, intersectionPoint, surfaceNormal);

		return info;
	}

	Intersect intersectTriangle(Ray &ray, parser::Face& face, vector<Vec3f>& vertexData){
		Vec3f a = vertexData[face.v0_id - 1];
		Vec3f b = vertexData[face.v1_id - 1];
		Vec3f c = vertexData[face.v2_id - 1];

		float detA = determinant(a - b, a - c, ray.direction);
		if(detA == 0) return Intersect(false);

		float t = determinant(a - b, a - c, a - ray.origin) / detA;
		float gamma = determinant(a - b, a - ray.origin, ray.direction) / detA;
		float beta = determinant(a - ray.origin, a - c, ray.direction) / detA;

		if(t > 0 && beta + gamma <= 1 && 0 <= beta && 0 <= gamma){
			Vec3f intersectionPoint = ray.origin + ray.direction*t;
			Vec3f surfaceNormal = (b - a).cross(c - a).normalize(); 
			Intersect info = Intersect(true, t, intersectionPoint, surfaceNormal);

			return info;
		}

		return Intersect(false);

	}
	bool findIntersection(Ray &ray, vector<Vec3f> &vertexData, parser::Scene &scene, Vec3f &intersectionPoint, Vec3f &surfaceNormal, 
																											int &materialId, float &minT ){

			for (int sphereIndex = 0; sphereIndex<scene.spheres.size(); sphereIndex++)
			{			
				Intersect info = intersectSphere(ray, scene.spheres[sphereIndex], vertexData);
				if (info.isHit && info.t < minT && info.t>0)
				{
					minT = info.t;
					materialId = scene.spheres[sphereIndex].material_id -1;
					intersectionPoint = info.intersectPoint;
					surfaceNormal = info.surfaceNormal;
				}
			}

			for(int triangleIndex = 0; triangleIndex < scene.triangles.size(); ++triangleIndex)
			{
				Intersect info = intersectTriangle(ray, scene.triangles[triangleIndex].indices, vertexData);

				if(info.isHit && info.t < minT && info.t>0){
					minT = info.t;
					materialId = scene.triangles[triangleIndex].material_id -1;
					intersectionPoint = info.intersectPoint;
					surfaceNormal = info.surfaceNormal;
				}
			}

			for(int meshIndex = 0; meshIndex < scene.meshes.size(); ++meshIndex){
				
				parser::Mesh mesh = scene.meshes[meshIndex];
				for(int faceIndex = 0; faceIndex < mesh.faces.size(); ++ faceIndex){
					Intersect info = intersectTriangle(ray, mesh.faces[faceIndex], vertexData);
					if(info.isHit && info.t < minT && info.t>0){
						minT = info.t;
						materialId = scene.meshes[meshIndex].material_id -1;
						intersectionPoint = info.intersectPoint;
						surfaceNormal = info.surfaceNormal;
					}					
				}
			}
			//If materialId is changed, then intersection is found;
			return materialId != -1;
	}

	Vec3f calculateColor(Ray &ray, parser::Scene &scene, parser::Camera &camera, int recursionDepth){
		int material, minI= -1, materialId = -1, placeholderI, shapeIndex, shape, placeholder;
		Vec3f color, intersectionPoint, surfaceNormal, ambient, diffuse, specular, placeholderV;
		float t, minT = 90000, tPointLight, placeholderF, tMinLight; // some large number
		bool isIntersected = false, isInShadow = false;
		//Intersect with spheres
		vector <parser::PointLight> pointLights = scene.point_lights;
		auto vertexData = scene.vertex_data;

		for(int indexLight = 0; indexLight < pointLights.size(); indexLight++){
			auto pointLight = scene.point_lights[indexLight];
			
			isIntersected = findIntersection(ray, vertexData, scene, intersectionPoint, surfaceNormal, materialId, minT);

			if (isIntersected){
				Vec3f wi = pointLight.position - intersectionPoint;
				wi = wi.normalize();
				Ray shadowRay(intersectionPoint + wi * scene.shadow_ray_epsilon, wi, true); //origin, direction
				isInShadow = findIntersection(shadowRay, vertexData, scene, placeholderV, placeholderV, placeholderI = -1, tMinLight = 90000);
				tPointLight = (pointLight.position - shadowRay.origin).x / shadowRay.direction.x;
				if(tMinLight > tPointLight || tPointLight < 0){
					isInShadow = false;
				}
			}
			if (isIntersected)
			{
				auto material = scene.materials[materialId];
				
				Vec3f ambientLight = scene.ambient_light;

				if(recursionDepth == scene.max_recursion_depth){
					color = color + calculateAmbiance(material, ambientLight);
				}				
				if(!isInShadow){
					Vec3f reflection;
					
					color = color + calculateDiffuse(material, pointLight, intersectionPoint, surfaceNormal);
					color = color + calculateSpecular(material, pointLight, intersectionPoint, surfaceNormal, ray.direction);

					if(recursionDepth > 0 && material.is_mirror && minT != 90000 && minT >0){
						Vec3f wo = (ray.direction * -1).normalize();
						Vec3f wr = ((surfaceNormal * 2) * (surfaceNormal * wo) -wo).normalize();
						Ray reflectionRay(intersectionPoint + wr * scene.shadow_ray_epsilon, wr, false);
						reflection = calculateColor(reflectionRay, scene, camera, recursionDepth-1);
					}
					color = color + scene.materials[materialId].mirror.scalar(reflection);
				}
			}
			else{
				color = Vec3f((float)scene.background_color.x, 
							(float)scene.background_color.y, 
							(float)scene.background_color.z);
			}

			materialId = -1;
			minT = 90000;
			isInShadow = false;
			isIntersected = false;
		}		
		return color;		
	}
}

#endif