#include <iostream>
#include <vector>
#include "parser.h"
#include "ppm.h"
#include "vec3f.h"
#include "Ray.h"
#include<string>
#include "utilizer.h"

using namespace utilizer;
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
    vector<Vec3f> fistik = scene.vertex_data;
    
    cameras = scene.cameras;
    for(auto camera : cameras){
         cout << "ilgaz" << endl;
        int width = camera.image_width;
        int height = camera.image_height;

        unsigned char* image = new unsigned char [width * height * 3];

        int pixel = 0;
        Ray ray(camera);
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                Vec3f color;
                ray = ray.generateRay(i, j, camera);
                color = calculateColor(ray, scene);
                image[pixel++] = color.x;
                image[pixel++] = color.y;
                image[pixel++] = color.z;
            }
        }
        const char * outputFileName = camera.image_name.c_str();
        write_ppm(outputFileName, image, width, height);
    }

}
