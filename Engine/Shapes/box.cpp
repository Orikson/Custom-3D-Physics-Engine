#include "box.h"

/**
 * Bounded Box constructor
 * @param dim Dimensions of the box
 * @param et_al see Shape constructor
 */
BBox::BBox(vec3 dim, float mass, vec3 com, vec4 orientation, float elasticity, bool anchor, vec3 color, int m, float refidx) : 
Shape(mass, com, orientation, elasticity, anchor, color, m, refidx), dim(dim) {
    // dim(l, h, w)
    float Ix = mass * (dim.Y()*dim.Y() + dim.Z()*dim.Z()) / 12;
    float Iy = mass * (dim.X()*dim.X() + dim.Z()*dim.Z()) / 12;
    float Iz = mass * (dim.X()*dim.X() + dim.Y()*dim.Y()) / 12;
    moment = mtrx3(
        vec3(Ix, 0, 0),
        vec3(0, Iy, 0),
        vec3(0, 0, Iz)
    );
    invMoment = moment.inverse();
}

/**
 * Returns the edge directions of the object. 
 */
vector<vec3> BBox::getEdges() const {
    vector<vec3> edges;
    vec3 rotdim = vec3::rotate(dim, rot);
    edges.insert(edges.end(), {
        vec3(rotdim.X(), 0, 0), 
        vec3(0, rotdim.Y(), 0), 
        vec3(0, 0, rotdim.Z())
    });
    return edges;
}

/**
 * Returns the projection of the shape onto a line
 * @param n Normalized direction of axis
 */
vec3 BBox::project(vec3 n) const {

}

/**
 * Calculate collision object between box (this) on sphere (shape)
 * @param shape Sphere to collide with
 * @param r Radius of given sphere
 */
void BBox::collideWith_Sphere(Collision* collision, const Shape& shape, float r) {
    bool col; 
    vec3 normal; 
    double penetration_depth; 
    vector<vec3> manifold;

    // should already be resolved
}
/**
 * Calculate collision object between box (this) on box (shape)
 * @param shape Box to collide with
 * @param dim Dimensions of given box
 */
void BBox::collideWith_Box(Collision* collision, const Shape& shape, vec3 dimensions) {
    bool col; 
    vec3 normal; 
    double penetration_depth; 
    vector<vec3> manifold;

    normal = SAT_boxBox(*this, shape, dim, dimensions);
    if (vec3::dot(normal, normal) == 0) {
        collision->col = false;
        return;
    }
    //cout << "\n----WOW!----\n";
    SAT_boxBoxCollision(collision, *this, shape, dim, dimensions);
    //cout << "\nCollision resulted in: n: "; vec3::printv3(collision->n);
    //cout << " pen: " << collision->pen << " manifold: "; 
    //for (vec3 man : collision->man) {
    //    vec3::printv3(man); cout << "; ";
    //}
    //collision->n = normal;
    //collision->pen = vec3::mag(pointToBox(collision->man.at(0), com, dim, rot));
    /*vector<vec3> tman;
    tman.push_back(com);
    collision->man = tman;
    collision->col = true;
    collision->pen = 0.1;*/
}
/**
 * Calculate collision object between box (this) on capsule (shape)
 * @param shape Capsule to collide with
 * @param len Length of given capsule
 * @param ri Inner radius of capsule
 * @param ro Placeholder for outer radius of capsule 
 */
void BBox::collideWith_Capsule(Collision* collision, const Shape& shape, float len, float ri, float ro) {
    bool col; 
    vec3 normal; 
    double penetration_depth; 
    vector<vec3> manifold;
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
vector<float> BBox::parseData() const {
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

vector<float> BBox::getVertices() const {
    vector<float> returned;
    return returned;
}