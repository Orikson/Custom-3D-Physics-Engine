#ifndef _COMMON_H
#define _COMMON_H

/*=======USEFUL MATH CONSTANTS=======*/
#define PI 3.141592653589793238462643383279502884197169399375105820974944592307816
#define G -9.8
#define DAMPEN 0.995


/*=======DATA CONSTANTS=======*/
// number of shapes
const int DSIZE = 6;
// width of data
const int WIDTH = 16;


/*=======STANDARD TEMPLATE LIBRARY=======*/
#include <stdio.h>
#include <stdlib.h> 
#include <stdexcept>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <memory>
#include <ctime>
#include <chrono>
#include <vector>
#include <math.h>
#include <sstream>
#include <time.h>
#include <tuple>
#include <limits>
#include <iomanip>
extern "C" {
    #include <unistd.h>
}

using namespace std;
namespace patch {
    template <typename T> string to_string(const T& n) {
        ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

template<typename Base, typename T> inline bool instanceof(const T*) {
    return std::is_base_of<Base, T>::value;
}


/*=======GLU/GLEW=======*/
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glut.h>


/*=======SDL2=======*/
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>


/*=======EXTERNAL CLASS DEFINITIONS=======*/
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


/*=======CLASS DEFINITIONS=======*/
#include "classes.h"

#include "Engine/Graphics/graphics.h"
#include "Engine/Vectors/vec3.h"
#include "Engine/Vectors/vec4.h"
#include "Engine/Vectors/mtrx3.h"

#include "Engine/Utility/collision.h"

#include "Engine/Shapes/shapes.h"
#include "Engine/Shapes/sphere.h"
#include "Engine/Shapes/box.h"
#include "Engine/Shapes/capsule.h"
#include "Engine/Shapes/mesh.h"

#endif