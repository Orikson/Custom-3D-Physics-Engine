#include "Engine/Kernel/kernel.h"
#include "Engine/Vectors/vec3.h"
#include "Engine/Vectors/vec4.h"
#include "Engine/Vectors/mtrx3.h"

int main(int argc, char* argv[])
{
    Kernel* kernel;
    kernel->start("3D Rendering", 500, 500);//argc, argv); 
    
    return 0;
}