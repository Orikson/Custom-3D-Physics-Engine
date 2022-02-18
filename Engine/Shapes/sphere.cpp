#include "sphere.h"

/**
 * Sphere constructor
 * @param r Radius of the sphere
 * @param et_al see Shape constructor
 */
Sphere::Sphere(float r, float mass, vec3 com, vec4 orientation, float elasticity, bool anchor, vec3 color, int m, float refidx) : 
Shape(mass, com, orientation, elasticity, anchor, color, m, refidx), r(r) {
    float tempMomentI = 2*mass*r*r/5;
    moment = mtrx3(
        vec3(tempMomentI, 0, 0),
        vec3(0, tempMomentI, 0),
        vec3(0, 0, tempMomentI)
    );
    invMoment = moment.inverse();
}

/**
 * Calculate collision object between sphere (this) on sphere (shape)
 * @param shape Sphere to collide with
 * @param radius Radius of given sphere
 */
Collision Sphere::collideWith_Sphere(const Shape& shape, float radius) {
    bool collision; 
    vec3 normal; 
    double penetration_depth; 
    vector<vec3> manifold;

    // collision if distance between centers <= sum of radii
    vec3 dir = shape.com - com;
    vec3 dirn = vec3::norm(dir);
    float dist = vec3::mag(dir);
    collision = (dist <= r + radius);

    if (!collision) {
        return Collision(false);
    }

    // contact point is middle of surface points
    vec3 s1 = com + dirn * r;
    vec3 s2 = shape.com - dirn * radius;
    manifold.push_back((s1 + s2) / 2);

    // collision normal is direction between centers
    normal = dirn;

    // penetration distance is distance between centers minus sum of radii
    penetration_depth = dist - r - radius;

    // concentric degenerate normal case
    if (vec3::mag(normal) == 0) {
        normal = vec3(0, 1, 0);
    }

    return Collision(true, normal, penetration_depth, manifold);
}
/**
 * Calculate collision object between sphere (this) on box (shape)
 * @param shape Box to collide with
 * @param dim Dimensions of given box
 */
Collision Sphere::collideWith_Box(const Shape& shape, vec3 dim) {
    bool collision; 
    vec3 normal; 
    double penetration_depth; 
    vector<vec3> manifold;
}
/**
 * Calculate collision object between sphere (this) on capsule (shape)
 * @param shape Capsule to collide with
 * @param len Length of given capsule
 * @param ri Inner radius of capsule
 * @param ro Placeholder for outer radius of capsule 
 */
Collision Sphere::collideWith_Capsule(const Shape& shape, float len, float ri, float ro) {
    bool collision; 
    vec3 normal; 
    double penetration_depth; 
    vector<vec3> manifold;
}


/*
 * Sphere Temp Table
 *              0           1           2           3           4           5           6           7           8           9           10          11          12          13          14          15
 * SHAPE NAME   SHAPE ID    |           |           |           |           |           |           |           |           |           |           |           |           |           |           |
 * Sphere       0           COM.X       COM.Y       COM.Z       ROT.X       ROT.Y       ROT.Z       ROT.W       RADIUS      ----        ----        MAT_ID       REF_IDX    COLOR.R     COLOR.G     COLOR.B
 */

/**
 * Parses a shape to an array of floats that correspond to a table found in "shapes.cpp"
 * @returns Float vector fitting the above description
 */
vector<float> Sphere::parseData() const {
    vector<float> returned{
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
        (float)m,
        refidx,
        color.X(),
        color.Y(),
        color.Z()
    };

    return returned;
}

vector<float> Sphere::getVertices() const {
    vector<float> returned;
    return returned;
}