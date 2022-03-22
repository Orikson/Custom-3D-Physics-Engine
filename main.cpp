#include "Engine/Kernel/kernel.h"

int main(int argc, char* argv[])
{
    Kernel* kernel;
    kernel->start("3D Rendering", 700, 700);//argc, argv); 
    
    return 0;
}