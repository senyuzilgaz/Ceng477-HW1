#include <iostream>
#include <vector>
#include "parser.h"
#include "ppm.h"
#include "vec3f.h"
#include "Ray.h"
#include<string>
#include "utilizer.h"
#include <chrono>
using namespace std::chrono;

using namespace utilizer;
using namespace std;
using namespace Vectors;
using namespace Rays;

typedef unsigned char RGB[3];



int main(int argc, char* argv[])
{
    auto start = high_resolution_clock::now();
    // Sample usage for reading an XML scene file
    parser::Scene scene;
    scene.loadFromXml(argv[1]);
    vector<parser::Camera> cameras;
    
    cameras = scene.cameras;


    

        for(auto camera : cameras){
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
                    ray = ray.generateRay(j, i, camera);
                    color = calculateColor(ray, scene, camera);
                    image[pixel++] = colorClamp(color.x);
                    image[pixel++] = colorClamp(color.y);
                    image[pixel++] = colorClamp(color.z);

                }
            }
            const char * outputFileName = camera.image_name.c_str();
            write_ppm(outputFileName, image, width, height);
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<seconds>(stop - start);
        cout << "Execution Time: " << duration.count() << " seconds " << endl;
}
