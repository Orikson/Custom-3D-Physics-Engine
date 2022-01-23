#include "collision.h"

Collision::Collision(bool collision, vec3 normal, double penetration_depth, vector<vec3> manifold) {
    col = collision;
    n = normal;
    pen = penetration_depth;
    man = manifold;
}
Collision::Collision(bool collision) {
    col = collision;
}