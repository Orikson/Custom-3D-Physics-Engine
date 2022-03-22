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
 * Returns the edges of the object. As spheres have no edges, returns nothing.
 */
vector<vec3> Sphere::getEdges() const {

}

/**
 * Returns the projection of the shape onto a line
 * @param n Normalized direction of axis
 */
vec3 Sphere::project(vec3 n) const {
    float tm = vec3::dot(com, n);
    return vec3(tm-r, tm+r, 0);
}

/**
 * Calculate collision object between sphere (this) on sphere (shape)
 * @param shape Sphere to collide with
 * @param radius Radius of given sphere
 */
void Sphere::collideWith_Sphere(Collision* collision, const Shape& shape, float radius) {
    bool col; 
    vec3 normal; 
    double penetration_depth; 
    vector<vec3> manifold;

    // collision if distance between centers <= sum of radii
    vec3 dir = shape.com - com;
    vec3 dirn = vec3::norm(dir);
    float dist = vec3::mag(dir);
    col = (dist <= r + radius);

    if (!col) {
        collision->col = false;
    } else {
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
        
        collision->col = true;
        collision->n = normal;
        collision->pen = penetration_depth;
        collision->man = manifold;
    }
}
/**
 * Calculate collision object between sphere (this) on box (shape)
 * @param shape Box to collide with
 * @param dim Dimensions of given box
 */
void Sphere::collideWith_Box(Collision* collision, const Shape& shape, vec3 dim) {
    bool col; 
    vec3 normal; 
    double penetration_depth; 
    vector<vec3> manifold;

    vector<float> sp = shape.parseData();
    
    // current SAT algorithm does a lot of arbitrary checks, whearas we can just check the axis of the shortest distance from the sphere to the box
    vec3 dist = pointToBox(com, shape.com, dim, shape.rot);
    
    if (vec3::dot(dist, dist) > r * r) {
        collision->col = false;
    } else {
        normal = vec3::norm(dist);
        vec3 p = com + dist;
        manifold.push_back(p);
        
        // deep penetration
        if (pointInBox(com, shape.com, dim, shape.rot)) {
            penetration_depth = vec3::mag(dist) + r;
            cout << "\ndeep\n";
        }
        else {
            penetration_depth = r - vec3::mag(dist);
            normal *= -1;
        }

        collision->col = true;
        collision->n = normal;
        collision->pen = penetration_depth;
        collision->man = manifold;
    }
}
/**
 * Calculate collision object between sphere (this) on capsule (shape)
 * @param shape Capsule to collide with
 * @param len Length of given capsule
 * @param ri Inner radius of capsule
 * @param ro Placeholder for outer radius of capsule 
 */
void Sphere::collideWith_Capsule(Collision* collision, const Shape& shape, float len, float ri, float ro) {
    bool col; 
    vec3 normal; 
    double penetration_depth; 
    vector<vec3> manifold;

    // capsule endpoints
    vec3 r1 = shape.com + vec3::rotate(vec3(0,  len, 0), shape.rot);
    vec3 r2 = shape.com + vec3::rotate(vec3(0, -len, 0), shape.rot);

    // closest point on defining ray
    vec3 dirn = vec3::shortestDistanceToLineSegment(com, r1, r2) * -1;
    vec3 L = com - dirn;

    // normal direction
    dirn = vec3::norm(dirn);
    float dist = vec3::mag(com - L);

    // collision if distance between relative segment center and sphere center less than the sum of the radii
    col = (dist <= ri + r);

    if (!col) {
        collision->col = false;
    } else {
        // contact point is middle of surface points
        vec3 s1 = com + dirn * r;
        vec3 s2 = L - dirn * ri;
        manifold.push_back((s1 + s2) / 2);

        // collision normal is direction between centers
        normal = dirn;

        // penetration distance is distance between centers minus sum of radii
        penetration_depth = dist - r - ri;

        // concentric degenerate normal case
        if (vec3::mag(normal) == 0) {
            normal = vec3(0, 1, 0);
        }

        collision->col = true;
        collision->n = normal;
        collision->pen = penetration_depth;
        collision->man = manifold;
    }
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