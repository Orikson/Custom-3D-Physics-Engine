#ifndef _COLLISION_H
#define _COLLISION_H

#include "../../common.h"

class Collision {
    public:
        Collision(bool collision, vec3 normal, double penetration_depth, vector<vec3> manifold);
        Collision(bool collision);
        
        bool col;
        vec3 n;
        double pen;
        vector<vec3> man;
};

#include "collision.cpp"

#endif