// Generic shape class
#ifndef _SHAPES_H
#define _SHAPES_H

#include "../../common.h"

class Shape {
    public:
        // all shapes have mass, a center of mass (the position!), orientation (the orientation!), a moment of inertia, and an elasticity value
        // graphical properties include color, material, and refraction index
        Shape(float mass, vec3 &com, vec4 &orientation, mtrx3 &moment, float elasticity, vec3 &color, int m, float refidx);

        void applyForce(vec3 n);
        void applyTorque(vec3 F, vec3 d);
        void updateLoop(float dT);

        // needs to be implemented per shape
        // returns an array of width WIDTH
        virtual float* parseData();

        // not private so parent classes can interact with them
        // physics properties
        vec3 sumF;
        vec3 sumT;

        vec3 com;
        vec3 linv;

        vec4 rot;
        float angv;

        mtrx3 moment;
        mtrx3 invMoment;

        float mass;
        float invMass;
        float e;

        // graphics properties
        vec3 color;
        
        int m;

        float refidx;
};


#include "shapes.cpp"

#endif