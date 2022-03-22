#include "capsule.h"

/**
 * Capsule constructor
 * @param length Length of capsule
 * @param radius Radius of swept sphere
 * @param et_al see Shape constructor
 */
Capsule::Capsule(float length, float radius, float mass, vec3 com, vec4 orientation, float elasticity, bool anchor, vec3 color, int m, float refidx) : 
Shape(mass, com, orientation, elasticity, anchor, color, m, refidx), l(length), r(radius) {
    // inertia of a cylinder/two hemispheres
    float tempmcy = l*r*r*PI;
    float tempmhs = (2/3)*r*r*r*PI;
    float tempm = tempmcy + 2*tempmhs;

    float mcy = mass*tempmcy/tempm;
    float mhs = mass*tempmhs/tempm;

    float Ix = mcy*(l*l/12 + r*r/4) + 2*mhs*(2*r*r/5 + l*l/2 + 3*l*r/8);
    float Iy = mcy*(r*r/2) + 2*mhs*(2*r*r/5);
    float Iz = mcy*(l*l/12 + r*r/4) + 2*mhs*(2*r*r/5 + l*l/2 + 3*l*r/8);
    moment = mtrx3(
        vec3(Ix, 0, 0),
        vec3(0, Iy, 0),
        vec3(0, 0, Iz)
    );
    invMoment = moment.inverse();
}

/**
 * Returns the edges of the object. As spheres have no edges, returns nothing.
 */
vector<vec3> Capsule::getEdges() const {

}

/**
 * Returns the projection of the shape onto a line
 * @param n Normalized direction of axis
 */
vec3 Capsule::project(vec3 n) const {

}

/**
 * Calculate collision object between capsule (this) on sphere (shape)
 * @param shape Sphere to collide with
 * @param r Radius of given sphere
 */
void Capsule::collideWith_Sphere(Collision* collision, const Shape& shape, float r) {
    bool col; 
    vec3 normal; 
    double penetration_depth; 
    vector<vec3> manifold;
}
/**
 * Calculate collision object between capsule (this) on box (shape)
 * @param shape Box to collide with
 * @param dim Dimensions of given box
 */
void Capsule::collideWith_Box(Collision* collision, const Shape& shape, vec3 dim) {
    bool col; 
    vec3 normal; 
    double penetration_depth; 
    vector<vec3> manifold;
}
/**
 * Calculate collision object between capsule (this) on capsule (shape)
 * @param shape Capsule to collide with
 * @param len Length of given capsule
 * @param ri Inner radius of capsule
 * @param ro Placeholder for outer radius of capsule 
 */
void Capsule::collideWith_Capsule(Collision* collision, const Shape& shape, float len, float ri, float ro) {
    bool col; 
    vec3 normal; 
    double penetration_depth; 
    vector<vec3> manifold;
}


/*
 * Box Temp Table
 *              0           1           2           3           4           5           6           7           8           9           10          11          12          13          14          15
 * SHAPE NAME   SHAPE ID    |           |           |           |           |           |           |           |           |           |           |           |           |           |           |
 * Capsule      2           COM.X       COM.Y       COM.Z       ROT.X       ROT.Y       ROT.Z       ROT.W       LENGTH      RADIUS      ----        MAT_ID      REF_IDX     COLOR.R     COLOR.G     COLOR.B
 */

/**
 * Parses a shape to an array of floats that correspond to a table found in "shapes.cpp"
 * @returns Float vector fitting the above description
 */
vector<float> Capsule::parseData() const {
    vector<float> returned{
        2,
        com.X(),
        com.Y(),
        com.Z(),
        rot.X(),
        rot.Y(),
        rot.Z(),
        rot.W(),
        l,
        r,
        0,
        (float)m,
        refidx,
        color.X(),
        color.Y(),
        color.Z()
    };

    return returned;
}

vector<float> Capsule::getVertices() const {
    vector<float> returned;
    return returned;
}