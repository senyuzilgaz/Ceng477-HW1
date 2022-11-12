#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "vec3f.h"

using namespace Vectors;
using namespace std;

int ffdmain(){
    Vec3f ilgaz(1.5, 2.31, 3.69);
    Vec3f talha(23.3, 0.2, 0.4);
    Vec3f cross = ilgaz.cross(talha);

    cout << cross;
}