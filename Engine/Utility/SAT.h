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

/**
 * Checks for a collision between two boxes given their dimensions and references
 * @param s1 First box corresponding with dim1
 * @param s2 Second box corresponding with dim2
 * @param dim1 Dimensions of the first box
 * @param dim2 Dimensions of the second box
 * @return vec3 representing the normal of a collision between the two boxes (0, 0, 0) if no collision detected
 */
vec3 SAT_boxBox(const Shape& s1, const Shape& s2, vec3 dim1, vec3 dim2) {
    vec3 w1 = vec3::rotate(vec3(dim1.X(), 0, 0), s1.rot);
    vec3 l1 = vec3::rotate(vec3(0, dim1.Y(), 0), s1.rot);
    vec3 h1 = vec3::rotate(vec3(0, 0, dim1.Z()), s1.rot);
    vec3 w2 = vec3::rotate(vec3(dim2.X(), 0, 0), s2.rot);
    vec3 l2 = vec3::rotate(vec3(0, dim2.Y(), 0), s2.rot);
    vec3 h2 = vec3::rotate(vec3(0, 0, dim2.Z()), s2.rot);
    vector<vec3> normals = {
        w1, l1, h1, 
        w2, l2, h2, 
        vec3::cross(w1, w2), vec3::cross(w1, l2), vec3::cross(w1, h2), 
        vec3::cross(l1, w2), vec3::cross(l1, l2), vec3::cross(l1, h2), 
        vec3::cross(h1, w2), vec3::cross(h1, l2), vec3::cross(h1, h2), 
    };
    vector<vec3> points1 = {
        s1.com + w1 + l1 + h1,
        s1.com + w1 + l1 - h1,
        s1.com + w1 - l1 + h1,
        s1.com + w1 - l1 - h1,
        s1.com + w1*-1 + l1 + h1,
        s1.com + w1*-1 + l1 - h1,
        s1.com + w1*-1 - l1 + h1,
        s1.com + w1*-1 - l1 - h1
    };
    vector<vec3> points2 = {
        s2.com + w2 + l2 + h2,
        s2.com + w2 + l2 - h2,
        s2.com + w2 - l2 + h2,
        s2.com + w2 - l2 - h2,
        s2.com + w2*-1 + l2 + h2,
        s2.com + w2*-1 + l2 - h2,
        s2.com + w2*-1 - l2 + h2,
        s2.com + w2*-1 - l2 - h2
    };
    vec3 minn = vec3(0);
    float mind = vec3::mag(dim1) + vec3::mag(dim2);
    for (vec3 n : normals) {
        float min1 = vec3::dot(points1.at(0), n);
        float max1 = min1;
        float min2 = vec3::dot(points2.at(0), n);
        float max2 = min2;

        for (vec3 v : points1) {
            float temp = vec3::dot(v, n);
            if (temp < min1) {
                min1 = temp;
            }
            if (temp > max1) {
                max1 = temp;
            }
        }
        for (vec3 v : points2) {
            float temp = vec3::dot(v, n);
            if (temp < min2) {
                min2 = temp;
            }
            if (temp > max2) {
                max2 = temp;
            }
        }
        
        // if any range is not intersecting then the objects are not colliding
        if (max(min1, min2) > min(max1, max2)) {
            return vec3(0);
        } else if (min(max1, max2) - max(min1, min2) < mind) {
            mind = min(max1, max2) - max(min1, min2);
            minn = n;
        }
    }
    // otherwise they are colliding
    return minn;
}

/**
 * Clips a ray defined by ro and rd based on whether the point exists on the side of the plane defined by n and originating from p
 * @return vec3 of the updated position of ro
 */
vec3 clipPoly(vec3 ro, vec3 rd, vec3 p, vec3 n) {
    vec3 tp = ro + rd;
    bool tn = vec3::dot(tp-p, n) > 0;
    bool pn = vec3::dot(ro-p, n) > 0;
    if (!pn || tn) {
        return ro;
    }

    // if ro is on the positive normal side of the plane readjust ro
    float t = vec3::dot(p - ro, n) / vec3::dot(rd, n);
    return ro + rd * t;
}

/**
 * Using relevant information only available within the SAT_boxBox collision detection function, update collision manifold
 */
void SAT_boxBoxCollision(Collision* collision, const Shape& s1, const Shape& s2, vec3 dim1, vec3 dim2) {
    vec3 w1 = vec3::rotate(vec3(dim1.X(), 0, 0), s1.rot);
    vec3 l1 = vec3::rotate(vec3(0, dim1.Y(), 0), s1.rot);
    vec3 h1 = vec3::rotate(vec3(0, 0, dim1.Z()), s1.rot);
    vec3 w2 = vec3::rotate(vec3(dim2.X(), 0, 0), s2.rot);
    vec3 l2 = vec3::rotate(vec3(0, dim2.Y(), 0), s2.rot);
    vec3 h2 = vec3::rotate(vec3(0, 0, dim2.Z()), s2.rot);
    vector<vec3> normals = {
        w1, l1, h1, 
        w2, l2, h2, 
        vec3::cross(w1, w2), vec3::cross(w1, l2), vec3::cross(w1, h2), 
        vec3::cross(l1, w2), vec3::cross(l1, l2), vec3::cross(l1, h2), 
        vec3::cross(h1, w2), vec3::cross(h1, l2), vec3::cross(h1, h2), 
    };
    vector<vec3> points1 = {
        s1.com + w1 + l1 + h1,
        s1.com + w1 + l1 - h1,
        s1.com + w1 - l1 + h1,
        s1.com + w1 - l1 - h1,
        s1.com + w1*-1 + l1 + h1,
        s1.com + w1*-1 + l1 - h1,
        s1.com + w1*-1 - l1 + h1,
        s1.com + w1*-1 - l1 - h1
    };
    vector<vec3> points2 = {
        s2.com + w2 + l2 + h2,
        s2.com + w2 + l2 - h2,
        s2.com + w2 - l2 + h2,
        s2.com + w2 - l2 - h2,
        s2.com + w2*-1 + l2 + h2,
        s2.com + w2*-1 + l2 - h2,
        s2.com + w2*-1 - l2 + h2,
        s2.com + w2*-1 - l2 - h2
    };
    vec3 minn = vec3(0);
    float mind = vec3::mag(dim1) + vec3::mag(dim2);
    for (vec3 norms : normals) {
        vec3 n = vec3::norm(norms);
        float min1 = vec3::dot(points1.at(0), n);
        float max1 = min1;
        float min2 = vec3::dot(points2.at(0), n);
        float max2 = min2;
        vec3 maxv1 = points1.at(0);
        vec3 maxv2 = points2.at(0);

        for (vec3 v : points1) {
            float temp = vec3::dot(v, n);
            if (temp < min1) {
                min1 = temp;
            }
            if (temp > max1) {
                max1 = temp;
                maxv1 = v;
            }
        }
        for (vec3 v : points2) {
            float temp = vec3::dot(v, n);
            if (temp < min2) {
                min2 = temp;
            }
            if (temp > max2) {
                max2 = temp;
                maxv2 = v;
            }
        }

        // if any range is not intersecting then the objects are not colliding
        if (max(min1, min2) > min(max1, max2)) {
            collision->col = false;
            return;
        } else if (min(max1, max2) - max(min1, min2) < mind) {
            mind = min(max1, max2) - max(min1, min2);
            minn = n;
            //cout << "\non normal: "; vec3::printv3(n);

        }
    }
    // we have found a collision!
    vec3 n = vec3::norm(minn);
    float pen = mind;
    //cout << "\nCollision using normal: "; vec3::printv3(n);

    // identify significant faces
    vector<vec3> s1normals = {w1, l1, h1, w1*-1, l1*-1, h1*-1};
    vector<vec3> s2normals = {w2, l2, h2, w2*-1, l2*-1, h2*-1};
    int cl1 = 0;
    float clmax1 = vec3::dot(vec3::norm(s1normals.at(0)), n);
    int cl2 = 0;
    float clmax2 = vec3::dot(vec3::norm(s2normals.at(0)), n*-1); 
    for (int i = 0; i < 6; i ++) {
        float t1 = vec3::dot(vec3::norm(s1normals.at(i)), n);
        float t2 = vec3::dot(vec3::norm(s2normals.at(i)), n*-1);
        float t3 = vec3::dot(vec3::norm(s1normals.at(i)), n*-1);
        float t4 = vec3::dot(vec3::norm(s2normals.at(i)), n);
        if (t1 > clmax1) {
            clmax1 = t1;
            cl1 = i;
        }
        if (t2 > clmax2) {
            clmax2 = t2;
            cl2 = i;
        }
        if (t3 > clmax1) {
            clmax1 = t3;
            cl1 = i;
        }
        if (t4 > clmax2) {
            clmax2 = t4;
            cl2 = i;
        }
    }

    bool or12 = false;

    // determine reference or incident face
    vec3 ref = vec3(0);
    vec3 inc = vec3(0);
    if (clmax1 > clmax2) {
        or12 = true;
        ref = s1normals.at(cl1);
        inc = s2normals.at(cl2);
    } else {
        or12 = false;
        ref = s2normals.at(cl2);
        inc = s1normals.at(cl1);
    }

    cout << "\nOr12: " << or12;
    cout << "\nUsing reference normal: "; vec3::printv3(ref); 
    cout << "\nUsing incident normal: "; vec3::printv3(inc);

    // clip by all adjacent planes
    // identify adjacent plane normal indicies
    // clip all points on incident plane until we have a set of points constructing our contact manifold
    // points consist of adjp sums (0, 1), (0, 3), (2, 3), (2, 1) [in this specific order according to gray binary]
    vector<vec3> points;
    vector<int> adjp = {1, 2, 4, 5};
    for (int i = 0; i < 4; i ++) {
        int offset = 0;
        if (or12) {
            offset = cl1;
        } else {
            offset = cl2;
        }
        adjp.at(i) = (offset + adjp.at(i)) % 6;
    }
    vec3 tcom = vec3(0);
    if (!or12) {
        tcom = s1.com;
        points.push_back(tcom + s1normals.at(adjp.at(0)) + s1normals.at(adjp.at(1)));
        points.push_back(tcom + s1normals.at(adjp.at(0)) + s1normals.at(adjp.at(3)));
        points.push_back(tcom + s1normals.at(adjp.at(2)) + s1normals.at(adjp.at(3)));
        points.push_back(tcom + s1normals.at(adjp.at(2)) + s1normals.at(adjp.at(1)));
    } else {
        tcom = s2.com;
        points.push_back(tcom + s2normals.at(adjp.at(0)) + s2normals.at(adjp.at(1)));
        points.push_back(tcom + s2normals.at(adjp.at(0)) + s2normals.at(adjp.at(3)));
        points.push_back(tcom + s2normals.at(adjp.at(2)) + s2normals.at(adjp.at(3)));
        points.push_back(tcom + s2normals.at(adjp.at(2)) + s2normals.at(adjp.at(1)));
    }
    
    cout << "\nUsing points: ";
    for (vec3 pointssss : points) {
        vec3::printv3(pointssss); cout << "; ";
    }
    cout << "\nTCOM: "; vec3::printv3(tcom);

    // now that we've determined the points on the incident plane, we must clip them
    // vec3 clipPoly(vec3 ro, vec3 rd, vec3 p, vec3 n)
    // iterate through normals
    for (int i = 0; i < 4; i ++) {
        // iterate through points and update
        for (int j = 0; j < 4; j ++) {
            cout << "\nClipping " << i << " " << j << " "; vec3::printv3(points.at(j)); cout << " -> ";
            if (or12) {
                vec3::printv3(clipPoly(points.at(j), points.at((j+4+1)%4) - points.at(j), tcom + s1normals.at(adjp.at(i)), s1normals.at(adjp.at(i))));
                points.at(j) = clipPoly(points.at(j), points.at((j+4+1)%4) - points.at(j), tcom + s1normals.at(adjp.at(i)), s1normals.at(adjp.at(i)));
                points.at(j) = clipPoly(points.at(j), points.at((j+4-1)%4) - points.at(j), tcom + s1normals.at(adjp.at(i)), s1normals.at(adjp.at(i)));
            } else {
                vec3::printv3(clipPoly(points.at(j), points.at((j+4+1)%4) - points.at(j), tcom + s2normals.at(adjp.at(i)), s2normals.at(adjp.at(i))));
                points.at(j) = clipPoly(points.at(j), points.at((j+4+1)%4) - points.at(j), tcom + s2normals.at(adjp.at(i)), s2normals.at(adjp.at(i)));
                points.at(j) = clipPoly(points.at(j), points.at((j+4-1)%4) - points.at(j), tcom + s2normals.at(adjp.at(i)), s2normals.at(adjp.at(i)));
            }
        }
    }
    
    // now we remove points still above the normal
    vector<vec3> final;
    for (int i = 0; i < 4; i ++) {
        vec3 tempv = points.at(i);
        if (vec3::dot(tempv - (tcom + ref), ref) < 0) {
            final.push_back(tempv);
        }
    }

    // finalize
    collision->col = true;
    collision->man = final;
    collision->pen = pen;
    collision->n = n;
    return;
}

#endif