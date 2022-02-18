// Sphere class
#ifndef _BOX_H
#define _BOX_H

#include "../../common.h"

class BBox: public Shape {
    public:
        // all shapes have mass, a center of mass (the position!), orientation (the orientation!), a moment of inertia, and an elasticity value
        // graphical properties include color, material, and refraction index
        BBox(vec3 dim, float mass, vec3 com, vec4 orientation, float elasticity, bool anchor, 
              vec3 color, int m, float refidx);

        // returns an array of width WIDTH
        vector<float> parseData() const override;

        // For meshes, can be ignored
        vector<float> getVertices() const override;

        // Collision functions
        Collision collideWith_Sphere(const Shape& shape, float r) override;
        Collision collideWith_Box(const Shape& shape, vec3 dim) override;
        Collision collideWith_Capsule(const Shape& shape, float len, float ri, float ro) override;

    private:
        vec3 dim;
};


#include "box.cpp"

#endif