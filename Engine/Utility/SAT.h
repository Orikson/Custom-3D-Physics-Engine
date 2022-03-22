#ifndef _SAT_H
#define _SAT_H

#include "../../common.h"

/**
 * Determines if a point is over (or under) a bounded plane
 * @param p Point
 * @param o Origin of plane
 * @param u First axis of plane
 * @param v Second axis of plane
 */
bool pointOverBoundedPlane(const vec3& p, const vec3& o, const vec3& u, const vec3& v) {
    vec3 t = p - o;
    return abs(vec3::dot(t, u)) <= vec3::dot(u, u) && 
           abs(vec3::dot(t, v)) <= vec3::dot(v, v);
}

/**
 * Determines if a point is inside a box
 * @param p Point
 * @param pos Position of box
 * @param dim Dimensions of box
 * @param rot Orientation of box
 * @return boolean representing whether or not the point is within the box
 */
bool pointInBox(const vec3& p, const vec3& pos, const vec3& dim, const vec4& rot) {
    vec3 w = vec3::rotate(vec3(dim.X(), 0, 0), rot);
    vec3 l = vec3::rotate(vec3(0, dim.Y(), 0), rot);
    vec3 h = vec3::rotate(vec3(0, 0, dim.Z()), rot);

    bool a = pointOverBoundedPlane(p, pos+w, l, h);
    bool b = pointOverBoundedPlane(p, pos+l, h, w);
    bool c = pointOverBoundedPlane(p, pos+h, w, l);

    return a && b && c;
}

/** 
 * Returns the vector representing the closest distance from a point to a bounded plane
 * @param p Point
 * @param o Origin of plane
 * @param u First axis of plane
 * @param v Second axis of plane
 */
vec3 pointToBoundedPlane(const vec3& p, const vec3& o, const vec3& u, const vec3& v) {
    vec3 t = p - o;
    if (abs(vec3::dot(t, u)) <= vec3::dot(u, u) &&
        abs(vec3::dot(t, v)) <= vec3::dot(v, v)) {
        vec3 n = vec3::cross(u, v);
        vec3 res = vec3::project(t, n) * -1;
        return res;
    }

    vec3 a = o + u + v;
    vec3 b = o + u - v;
    vec3 c = o - u - v;
    vec3 d = o - u + v;

    vec3 e = vec3::shortestDistanceToLineSegment(p, a, b);
    vec3 f = vec3::shortestDistanceToLineSegment(p, b, c);
    vec3 g = vec3::shortestDistanceToLineSegment(p, c, d);
    vec3 h = vec3::shortestDistanceToLineSegment(p, d, a);

    float me = vec3::mag(e);
    float mf = vec3::mag(f);
    float mg = vec3::mag(g);
    float mh = vec3::mag(h);

    float tp = min(me, min(mf, min(mg, mh)));
    if (tp == me) {
        return e;
    } else if (tp == mf) {
        return f;
    } else if (tp == mg) {
        return g;
    }
    return h;
}

/**
 * Returns the vector representing the closest distance from a point to a box
 * @param p Point
 * @param pos Box position
 * @param dim Box dimensions
 * @param rot Box orientation
 */
vec3 pointToBox(const vec3& p, const vec3& pos, const vec3& dim, const vec4& rot) {
    vec3 w = vec3::rotate(vec3(dim.X(), 0, 0), rot);
    vec3 l = vec3::rotate(vec3(0, dim.Y(), 0), rot);
    vec3 h = vec3::rotate(vec3(0, 0, dim.Z()), rot);

    vec3 a = pointToBoundedPlane(p, pos+w, l, h);
    vec3 b = pointToBoundedPlane(p, pos+l, h, w);
    vec3 c = pointToBoundedPlane(p, pos+h, w, l);

    vec3 d = pointToBoundedPlane(p, pos-w, l*-1, h*-1);
    vec3 e = pointToBoundedPlane(p, pos-l, h*-1, w*-1);
    vec3 f = pointToBoundedPlane(p, pos-h, w*-1, l*-1);

    float ma = vec3::mag(a);
    float mb = vec3::mag(b);
    float mc = vec3::mag(c);
    float md = vec3::mag(d);
    float me = vec3::mag(e);
    float mf = vec3::mag(f);

    float tp = min(ma, min(mb, min(mc, min(md, min(me, mf)))));

    if (tp == ma) {
        return a;
    } else if (tp == mb) {
        return b;
    } else if (tp == mc) {
        return c;
    } else if (tp == md) {
        return d;
    } else if (tp == me) {
        return e;
    }
    return f;
}

/**
 * Checks for a collision between two convex shapes (but does not check whether the provided shapes are indeed convex)
 * @param s1 First shape
 * @param s2 Second shape
 * @return Whether or not the two convex shapes collide
 */
bool SAT_checkCollision(const Shape& s1, const Shape& s2) {
    // first we identify the set of all edges
    vector<vec3> e1 = s1.getEdges();
    vector<vec3> e2 = s2.getEdges();

    vector<float> s1p = s1.parseData();
    vector<float> s2p = s2.parseData();

    float s1t = s1p.at(0);
    float s2t = s2p.at(0);

    // if first shape is a sphere, its "edges" are defined by vector representing the shortest distance between the two objects
    if (s1t == 0) {
        // if second shape is also a sphere, subtract centers
        if (s2t == 0) {
            vec3 n = s1.com - s2.com;
            n = vec3::norm(n);

            e1.push_back(n);
            // don't need to push to second, as it will be a duplicate axis to check
        } 
        // if second shape is a box, do some math
        else if (s2t == 1) {
            vec3 n = pointToBox(s1.com, s2.com, vec3(s2p.at(8), s2p.at(9), s2p.at(10)), s2.rot);
            n = vec3::norm(n);

            e1.push_back(n);
        }
        // if second shape is a capsule, do some math
        else if (s2t == 2) {
            vec3 a = s2.com + vec3::rotate(vec3(0, s2p.at(8), 0), s2.rot);
            vec3 b = s2.com + vec3::rotate(vec3(0, -s2p.at(8), 0), s2.rot);
            vec3 n = vec3::shortestDistanceToLineSegment(s1.com, a, b);
            n = vec3::norm(n);

            e1.push_back(n);
        }
    }
    // if first shape is a capsule, ...
    else if (s1t == 1) {
        
    }
}

#endif