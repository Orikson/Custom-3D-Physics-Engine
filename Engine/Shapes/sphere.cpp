#include "sphere.h"

/**
 * Sphere constructor
 * @param r Radius of the sphere
 * @param et_al see Shape constructor
 */
Sphere::Sphere(float r, float mass, vec3 com, vec4 orientation, mtrx3 I, float elasticity, vec3 color, int m, float refidx) : 
Shape(mass, com, orientation, I, elasticity, color, m, refidx), r(r) {

}


/*
 * Sphere Temp Table
 *              0           1           2           3           4           5           6           7           8           9           10          11          12          13          14          15
 * SHAPE NAME   SHAPE ID    |           |           |           |           |           |           |           |           |           |           |           |           |           |           |
 * Sphere       0           COM.X       COM.Y       COM.Z       ROT.X       ROT.Y       ROT.Z       ROT.W       RADIUS      ----        ----        MAT_ID       REF_IDX    COLOR.R     COLOR.G     COLOR.B
 */

/**
 * Parses a shape to an array of floats that correspond to a table found in "shapes.cpp"
 * @returns Float array fitting the above description
 */
float* Sphere::parseData() {
    float returned[WIDTH] = {
        0,
        com.X(),
        com.Y(),
        com.Z(),
        rot.X(),
        rot.Y(),
        rot.Z(),
        rot.W(),
        r,
        0,
        0,
        m,
        refidx,
        color.X(),
        color.Y(),
        color.Z()
    };

    return returned;
}