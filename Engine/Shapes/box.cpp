#include "box.h"

/**
 * Bounded Box constructor
 * @param dim Dimensions of the box
 * @param et_al see Shape constructor
 */
BBox::BBox(vec3 dim, float mass, vec3 com, vec4 orientation, float elasticity, vec3 color, int m, float refidx) : 
Shape(mass, com, orientation, elasticity, color, m, refidx), dim(dim) {

}


/*
 * Box Temp Table
 *              0           1           2           3           4           5           6           7           8           9           10          11          12          13          14          15
 * SHAPE NAME   SHAPE ID    |           |           |           |           |           |           |           |           |           |           |           |           |           |           |
 * Box          1           COM.X       COM.Y       COM.Z       ROT.X       ROT.Y       ROT.Z       ROT.W       DIM.X       DIM.Y       DIM.Z       MAT_ID       REF_IDX    COLOR.R     COLOR.G     COLOR.B
 */

/**
 * Parses a shape to an array of floats that correspond to a table found in "shapes.cpp"
 * @returns Float vector fitting the above description
 */
vector<float> BBox::parseData() {
    vector<float> returned{
        1,
        com.X(),
        com.Y(),
        com.Z(),
        rot.X(),
        rot.Y(),
        rot.Z(),
        rot.W(),
        dim.X(),
        dim.Y(),
        dim.Z(),
        (float)m,
        refidx,
        color.X(),
        color.Y(),
        color.Z()
    };

    return returned;
}