#include <iostream>
#include <vector>
#include "parser.h"
#include "ppm.h"
#include "vec3f.h"
#include "Ray.h"
#include<string>

using namespace std;
using namespace Vectors;
using namespace Rays;

typedef unsigned char RGB[3];



int main(int argc, char* argv[])
{
    // Sample usage for reading an XML scene file
    parser::Scene scene;
    scene.loadFromXml(argv[1]);
    vector<parser::Camera> cameras;

    
    cameras = scene.cameras;
    parser::Camera camera = cameras[0];
    
    int width = camera.image_width;
    int height = camera.image_height;

    unsigned char* image = new unsigned char [width * height * 3];
    Vectors::Vec3f talha;

    int px = 0;
    Ray ray(camera);
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            ray = ray.generateRay(j, i, camera);
            talha = ray.direction + ray.origin;
            cout << talha;
            //image[pixel++] = 0;
            //image[pixel++] = 0;
            //image[pixel++] = 0;
        }
    }

    write_ppm("test.ppm", image, width, height);

}
