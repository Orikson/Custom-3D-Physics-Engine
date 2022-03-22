// Box class
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

        // functions to help with standard collision detection algorithms
        vector<vec3> getEdges() const override;
        vec3 project(vec3 n) const override;

        // Collision functions
        void collideWith_Sphere(Collision* collision, const Shape& shape, float r) override;
        void collideWith_Box(Collision* collision, const Shape& shape, vec3 dim) override;
        void collideWith_Capsule(Collision* collision, const Shape& shape, float len, float ri, float ro) override;

    private:
        vec3 dim;
};


#include "box.cpp"

#endif