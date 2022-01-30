// Sphere class
#ifndef _SPHERE_H
#define _SPHERE_H

#include "../../common.h"

class Sphere: public Shape {
    public:
        // all shapes have mass, a center of mass (the position!), orientation (the orientation!), a moment of inertia, and an elasticity value
        // graphical properties include color, material, and refraction index
        Sphere(float r, float mass, vec3 com, vec4 orientation, float elasticity,
              vec3 color, int m, float refidx);

        // returns an array of width WIDTH
        vector<float> parseData() override;

        // Collision functions
        Collision collideWith_Sphere(const Sphere& sphere);
        Collision collideWith_Box(const BBox& box);
        Collision collideWith_Capsule(const Capsule& capsule);

    private:
        float r;
};


#include "sphere.cpp"

#endif