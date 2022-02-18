#include "shapes.h"

/** ----- DEFINING PARSING TABLE ----- 
 *              0           1           2           3           4           5           6           7           8           9           10          11          12          13          14          15
 * SHAPE NAME   SHAPE ID    |           |           |           |           |           |           |           |           |           |           |           |           |           |           |
 * Sphere       0           COM.X       COM.Y       COM.Z       ROT.X       ROT.Y       ROT.Z       ROT.W       RADIUS      ----        ----        MAT_ID      REF_IDX     COLOR.R     COLOR.G     COLOR.B
 * Box          1           COM.X       COM.Y       COM.Z       ROT.X       ROT.Y       ROT.Z       ROT.W       DIM.X       DIM.Y       DIM.Z       MAT_ID      REF_IDX     COLOR.R     COLOR.G     COLOR.B
 * Capsule      2           COM.X       COM.Y       COM.Z       ROT.X       ROT.Y       ROT.Z       ROT.W       LENGTH      RADIUS      ----        MAT_ID      REF_IDX     COLOR.R     COLOR.G     COLOR.B
 * Mesh         3           COM.X       COM.Y       COM.Z       ROT.X       ROT.Y       ROT.Z       ROT.W       MESH_SIZE   LARGEST_D   MESH_INDX   MAT_ID      REF_IDX     COLOR.R     COLOR.G     COLOR.B
 * ...
 */

/** ----- DEFINING MATERIAL TABLE -----
 * MAT NAME     MAT ID
 * Lambertian   0
 * Metal        1
 * Glass        2
 * ...
 */

/**
 * Shape constructor
 * @param mass Numerical mass of the object
 * @param com Location of the center of mass in the world (the location of the center of mass within the shape is predefined per shape)
 * @param orientation Quaternion representing the initial orientation of the object
 * @param I Moment of inertia matrix
 * @param elasticity Elasticity of the object
 * @param color Surface color of the object
 * @param m Material identifier of the object (as described in a table within "shapes.cpp")
 * @param refidx Refraction index of the object (only used for materials of type glass)
 */
Shape::Shape(float mass, vec3 &com, vec4 &orientation, float elasticity, bool anchor, vec3 &color, int m, float refidx) : 
    mass(mass), com(com), rot(orientation), e(elasticity), anchor(anchor), color(color), m(m), refidx(refidx) {
    sumF = vec3(0);
    sumT = vec3(0);

    invMass = 1/mass;
}

/**
 * Applies a force of given direction and magnitude
 * @param n Description of force
 */
void Shape::applyForce(vec3 n) {
    sumF += n;
}

/**
 * Applies a torque given a force applied at a given displacement from the object
 * @param F Force applied
 * @param d Displacement from the center of mass to the position of the force applied
 */
void Shape::applyTorque(vec3 F, vec3 d) {
    sumT += vec3::cross(F, d);
}

/**
 * OVERRIDED: Shape overrided function per specific shape. Used to parse shape data to pass to the renderer
 */
vector<float> Shape::parseData() const {}
vector<float> Shape::getVertices() const { vector<float> returned; return returned; }
Collision Shape::collideWith_Sphere(const Shape& shape, float r) {}
Collision Shape::collideWith_Box(const Shape& shape, vec3 dim) {}
Collision Shape::collideWith_Capsule(const Shape& capsule, float len, float ri, float ro) {}


/**
 * Calculate resultant velocities of a potential collision with a given object
 * @param shape shape to check a collision with
 */
void Shape::collideWith(const Shape& shape) {
    vector<float> tempData;
    tempData = shape.parseData();

    Collision res;

    switch((int)tempData.at(0)) {
        case 0: // sphere
            res = collideWith_Sphere(shape, tempData.at(8));
            break;
        case 1: // box

            break;
        case 2: // capsule

            break;
        case 3: // mesh

            break;
        default:
            break;
    }

}

/**
 * Standard shape update loop
 * @param dT time difference from previous loop update
 */
void Shape::updateLoop(float dT) {
    if (!anchor) {
        // apply gravity
        applyForce(vec3(0, G, 0));

        // update velocity
        linv += sumF * invMass * dT;

        // dampen velocity
        linv *= DAMPEN;

        // update angular velocity
        angv += invMoment * sumT * dT;
        
        // dampen angular velocity
        angv *= DAMPEN;

        // update position
        com += linv * dT;

        // update orientation
        vec3 temp = angv * dT * 0.5;
        rot += vec4(temp.X(), temp.Y(), temp.Z(), 0) * rot;
        
        // normalize orientation
        rot = vec4::norm(rot);

        // reset sum of forces
        sumF = vec3(0);
        sumT = vec3(0);
    }
}
