// Generic shape class
#ifndef _SHAPES_H
#define _SHAPES_H

#include "../../common.h"

class Shape {
    public:
        // all shapes have mass, a center of mass (the position!), orientation (the orientation!), a moment of inertia, an elasticity value, and whether or not the object is immobilized
        // graphical properties include color, material, and refraction index
        Shape(float mass, vec3 &com, vec4 &orientation, float elasticity, bool anchor, vec3 &color, int m, float refidx);

        void applyForce(vec3 n);
        void applyTorque(vec3 F, vec3 d);
        void updateLoop(float dT);

        // needs to be implemented per shape
        // returns an array of width WIDTH
        virtual vector<float> parseData() const;

        // a function just for meshes
        virtual vector<float> getVertices() const;

        // update collisions with a shape (uses parseData to access data it would otherwise not know about)
        void collideWith(const Shape& shape);
        virtual Collision collideWith_Sphere(const Shape& shape, float r);
        virtual Collision collideWith_Box(const Shape& shape, vec3 dim);
        virtual Collision collideWith_Capsule(const Shape& capsule, float len, float ri, float ro);

        // not private so parent classes can interact with them
        // physics properties
        vec3 sumF;
        vec3 sumT;

        vec3 com;
        vec3 linv;

        vec4 rot;
        vec3 angv;

        mtrx3 moment;
        mtrx3 invMoment;

        float mass;
        float invMass;
        float e;

        bool anchor;

        // graphics properties
        vec3 color;
        
        int m;

        float refidx;
};


#include "shapes.cpp"

#endif