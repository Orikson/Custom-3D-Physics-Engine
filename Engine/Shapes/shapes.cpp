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
vector<vec3> Shape::getEdges() const {}
vec3 Shape::project(vec3 n) const {}
void Shape::collideWith_Sphere(Collision* collision, const Shape& shape, float r) {}
void Shape::collideWith_Box(Collision* collision, const Shape& shape, vec3 dim) {}
void Shape::collideWith_Capsule(Collision* collision, const Shape& capsule, float len, float ri, float ro) {}


/**
 * Calculate resultant velocities of a potential collision with a given object
 * @param shape shape to check a collision with
 */
void Shape::collideWith(Shape* shape, float dT) {
    vector<float> tempData;
    tempData = shape->parseData();

    // if its an anchored shape, no need to check collisions (other objects will check collisions with it)
    if (anchor) {
        return;
    }

    Collision res;

    switch((int)tempData.at(0)) {
        case 0: // sphere
            collideWith_Sphere(&res, *shape, tempData.at(8));
            break;
        case 1: // box
            collideWith_Box(&res, *shape, vec3(tempData.at(8), tempData.at(9), tempData.at(10)));
            break;
        case 2: // capsule
            collideWith_Capsule(&res, *shape, tempData.at(8), tempData.at(9), 0);
            break;
        case 3: // mesh
            cout << "\n!?";
            break;
        default:
            cout << "\n!!";
            break;
    }

    //cout << "\n\nShape " << tempData.at(0) << " " << res.col;

    if (res.col) {
        float elasticity = e*shape->e;
        
        // consider per point in contact manifold penetration depth
        // for now, considering the maximum penetration depth as the points penetration depth
        float penSlop = min(SLOP + res.pen, 0.0);
        cout << "\nUsing: ";
        for (vec3 contact : res.man) {
            vec3::printv3(contact); cout << "\n\t";
        }
        for (vec3 contact : res.man) {
            vec3 ra = contact - com;
            vec3 rb = contact - shape->com;

            float bterm = -(BAUMGARTE / dT) * penSlop;

            float eterm = vec3::dot(res.n, linv + vec3::cross(ra, angv) - shape->linv - vec3::cross(rb, shape->angv));

            bterm += (elasticity * eterm) / res.man.size();

            bterm = 0;

            // velocities of each point
            vec3 v0 = linv + vec3::cross(angv, ra);
            vec3 v1 = shape->linv + vec3::cross(shape->angv, rb);
            vec3 dv = v1 - v0;

            // constraint mass
            float cmass;
            if (shape->anchor) {
                cmass = invMass +
                    vec3::dot(res.n, 
                        vec3::cross(invMoment*vec3::cross(ra, res.n), ra)
                    );
            } else {
                cmass = invMass + shape->invMass +
                    vec3::dot(res.n, 
                        vec3::cross(invMoment*vec3::cross(ra, res.n), ra) +
                        vec3::cross(shape->invMoment*vec3::cross(rb, res.n), rb)
                    );
            }

            if (cmass > 0) {
                float jn = max(vec3::dot(dv, res.n) * elasticity + bterm, 0.0f);
                jn /= cmass;

                float multj = 1;
                float multv = 2;
                if (!shape->anchor) {
                    multj = 0.5;
                    multv = 1;
                }

                com += res.n * res.pen * multj;
                linv += res.n * jn * invMass * multv;
                angv += invMoment * vec3::cross(ra, res.n * jn * multv);
                
                if (!shape->anchor) {
                    shape->com -= res.n * res.pen * multj;
                    shape->linv -= res.n * jn * invMass;
                    shape->angv -= shape->invMoment * vec3::cross(rb, res.n * jn);
                }
            }
            
            
            /*
            vec3 vab = (linv + vec3::cross(angv, ra)) - (shape->linv + vec3::cross(shape->angv, rb));
            float Jtop = -(1+shape->e*e)*(vec3::dot(vab, res.n));
            float Jbot = (vec3::dot(res.n, res.n)*(invMass + shape->invMass));
            vec3 ta = vec3::cross(invMoment * vec3::cross(ra, res.n), ra);
            vec3 tb = vec3::cross(shape->invMoment * vec3::cross(rb, res.n), rb);
            Jbot += vec3::dot(ta + tb, res.n);

            float J = Jtop / Jbot;


            if (shape->anchor) {
                com += res.n * res.pen;
                linv += res.n * J * invMass;
                angv -= invMoment * vec3::cross(ra, (res.n * J));
            } else {
                com += res.n * res.pen / 2;
                shape->com -= res.n * res.pen / 2;
                linv += res.n * J * invMass;
                shape->linv -= res.n * J * shape->invMass;
                angv += invMoment * vec3::cross(ra, (res.n * J));
                shape->angv -= shape->invMoment * vec3::cross(rb, (res.n * J));
            }
            */
        }
    }
}

/**
 * Standard shape update loop
 * @param dT time difference from previous loop update
 */
void Shape::updateLoop(float dT) {
    if (!anchor) {
        // apply gravity
        applyForce(vec3(0, mass * G, 0));

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
        rot += rot * vec4(0, temp.X(), temp.Y(), temp.Z());
        
        // normalize orientation
        rot = vec4::norm(rot);

        // reset sum of forces
        sumF = vec3(0);
        sumT = vec3(0);
    }
}
