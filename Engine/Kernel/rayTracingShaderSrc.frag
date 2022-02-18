#version 430

// float coordinates defined by vertex shader (between -1 and 1)
varying float x, y, z;
uniform int iFrame;
uniform float iTime;
uniform vec3 cPos;
uniform vec3 cRot;

#define MAXD 100.0
#define MAXT 100.0
#define MINT 0.001
#define MAXBOUNCES 15
#define MAXSAMPLES 10

// visual constants
#define FUZZ 0.001

// layouts
layout (std430, binding=2) buffer shader_data
{ 
	vec2 res;
	int size;
	int width;
	float data[];
};

uint rngstate = uint(1);

uint randInt() {
    rngstate ^= rngstate << 13;
    rngstate ^= rngstate >> 17;
	rngstate ^= rngstate << 5;
    return rngstate;
}

void setSeed(uint seed) {
    rngstate = seed;
    randInt(); randInt(); randInt(); // Shift some bits around
}

float randFloat() {
    randInt(); randInt(); randInt(); // Shift bits ;D
    return fract(float(randInt()) / float(1<<32 - 5));
}

vec3 randDir(vec3 n) {
    float u = randFloat();
    float v = randFloat();

    float theta = 2.0*3.1415*u;
    float phi = acos(2.0*v-1.0);
    
    vec3 res = vec3(sin(phi)*cos(theta), sin(phi)*sin(theta), cos(phi));
    
    return sign(dot(n, res))*res;
}
vec3 randDir() {
    float u = randFloat();
    float v = randFloat();

    float theta = 2.0*3.1415*u;
    float phi = acos(2.0*v-1.0);
    
    return vec3(sin(phi)*cos(theta), sin(phi)*sin(theta), cos(phi));
}
vec3 randInUnitSphere() {
    vec3 d = randDir();
    float m = randFloat();

    return d*m;
}
float hash12(vec2 p) {
	vec3 p3  = fract(vec3(p.xyx) * .1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.x + p3.y) * p3.z);
}

struct ray {
    vec3 ro;    // ray origin
    vec3 rd;    // ray direction
};

struct collision {
    float t;    // time to collision
    vec3 p;     // position of collision
    vec3 n;     // normal at collision
    vec3 obc;   // object center
    int obi;    // object index
};

// Hamilton Product
vec4 H(vec4 n1, vec4 n2) {
    float a1 = n1.x; float a2 = n2.x;
    float b1 = n1.y; float b2 = n2.y;
    float c1 = n1.z; float c2 = n2.z;
    float d1 = n1.w; float d2 = n2.w;

    return vec4(
        a1*a2-b1*b2-c1*c2-d1*d2,
        a1*b2+b1*a2+c1*d2-d1*c2,
        a1*c2-b1*d2+c1*a2+d1*b2,
        a1*d2+b1*c2-c1*b2+d1*a2
    );
}

/* Usage:
vec3 n - position to rotate
vec4 rot - rotation quaternion

quaternion defined by:

rotate around normalize(vec3(X, Y, Z))
C = cos (theta/2)
S = sin (theta/2)
rot = vec4(C, X*S, Y*S, Z*S)
*/
vec3 rotate(vec3 n, vec4 rot) {
    vec4 quantN = vec4(0, n);
    vec4 rotPrime = vec4(rot.x, -rot.yzw);

    vec4 quantNPrime = H(H(rot, quantN), rotPrime);

    return quantNPrime.yzw;
}

vec4 quaternion(vec3 n, float theta) {
    return vec4(cos(theta/2), sin(theta/2)*n);
}

// basically because we have a shape at a certain position with a certain rotation (and we only know how to find distances to objects with default orientation)
//      we have to revert the rotation (by applying the reverse rotation to the current position around the given "center" of the object)
vec3 translate(vec3 curpos, vec3 pos, vec4 rot) {
    // vector from position of the object to current position
    vec3 orig = curpos - pos;

    // we know the first component of quaternion must be cos (theta/2)
    // we want to rotate by the negative of this which so happens to be
    // vec4 invrot = vec4(rot.x, -rot.yzw);
    // because cos (-theta/2) = cos (theta/2), sin (-theta/2) = -sin (theta/2)
    vec4 invrot = vec4(rot.x, -rot.yzw);

    // rotate curpos by the inverse rotation around pos
    vec3 res = rotate(orig, invrot);

    return pos + res;
}

// however we mainly are going to use this on a ray, so we somehow have to update a ray to approach this singular object 

// ray, sphere position/radius/orientation, collision
void collide_sphere(ray ry, vec3 pos, float r, vec4 rot, int index, inout collision col) {
    // not using rot simply because textures haven't been implemented yet (and otherwise spheres are uniformly symmetrical)
    vec3 C = pos;
    float a = dot(ry.rd, ry.rd);
    float b = -2.*dot(ry.rd, C-ry.ro);
    float c = pow(length(C-ry.ro),2.)-pow(r,2.);

    if (pow(b,2.)-4.*a*c >= 0.) {
        float t1 = (-b + sqrt(pow(b,2.)-4.*a*c))/(2.*a);
        float t2 = (-b - sqrt(pow(b,2.)-4.*a*c))/(2.*a);
        
        bool coll = false;
        float t;
        if (t1 < MINT) {
            if (t2 < MINT) {
                coll = false;
            } else {
                coll = true;
                t = t2;
            }
        } else {
            if (t2 < MINT) {
                coll = true;
                t = t1;
            } else {
                coll = true;
                t = min(t1, t2);
            }
        }

        if (coll && t < col.t) {
            col.t = t;
            col.p = ry.ro + ry.rd*col.t;
            col.n = normalize(col.p-pos)*sign(r);
            col.obc = pos;
            col.obi = index;
        }
    }
}

// ray, plane normal/point on plane
void collide_plane(ray ry, vec3 n, vec3 p, int index, inout collision col) {
    // if parallel approx 0 (for error < 0.001)
    if (abs(dot(n, ry.rd)) > 0.001) {
        float tempT = dot(p-ry.ro, n)/dot(ry.rd, n);
        
        if (tempT < col.t && tempT > MINT) {
            col.t = tempT;
            col.p = ry.ro + ry.rd*col.t;
            col.n = n;
            col.obc = p;
            col.obi = index;
        }
    }
}

// ray, point on plane/uv vectors defining bounds
void collide_boundedPlane(ray ry, vec3 p, vec3 u, vec3 v, vec3 n, int index, inout collision col) {
    // if parallel approx 0 (for error < 0.001)
    if (abs(dot(n, ry.rd)) > 0.001) {
        float tempT = dot(p-ry.ro, n)/dot(ry.rd, n);
        vec3 tempP = ry.ro + ry.rd*tempT;

        vec3 a = tempP - p;

        if (tempT < col.t && tempT > MINT && dot(a, v) < dot(v, v) && dot(a, u) < dot(u, u) && -dot(a, v) < dot(v, v) && -dot(a, u) < dot(u, u)) {
            col.t = tempT;
            col.p = tempP;
            col.n = n;
            col.obc = p;
            col.obi = index;
        }
    }
}

// ray, 3 points on plane
void collide_boundedTriangle(ray ry, vec3 p, vec3 q, vec3 r, int index, inout collision col) {
    vec3 n = normalize(cross(q-p, r-p));
    
    // if parallel approx 0 (for error < 0.001)
    if (abs(dot(n, ry.rd)) > 0.001) {
        float tempT = dot(p-ry.ro, n)/dot(ry.rd, n);
        vec3 tempP = ry.ro + ry.rd*tempT;

        vec3 a = tempP - p;
        vec3 b = tempP - q;
        vec3 c = tempP - r;

        bool ta = dot(cross(q-p, a), n) >= 0;
        bool tb = dot(cross(r-q, b), n) >= 0;
        bool tc = dot(cross(p-r, c), n) >= 0;

        if (tempT < col.t && tempT > MINT && ta && tb && tc) {
            col.t = tempT;
            col.p = tempP;
            col.n = n;
            col.obc = p;
            col.obi = index;
        }
    }
}

// ray, box position/dimensions/orientation, collision
void collide_box(ray ry, vec3 pos, vec3 b, vec4 rot, int index, inout collision col) {
    vec3 defL = vec3(1, 0, 0);
    vec3 defU = vec3(0, 1, 0);
    vec3 defV = vec3(0, 0, 1);

    vec3 rotL = rotate(defL, rot)*b.x;
    vec3 rotU = rotate(defU, rot)*b.y;
    vec3 rotV = rotate(defV, rot)*b.z;

    vec3 negL = -rotL;
    vec3 negU = -rotU;
    vec3 negV = -rotV;

    // need to test all combinations of +rot and -neg (secluded combinations)
    // rotL rotU
    collide_boundedPlane(ry, pos+rotV, rotL, rotU, rotV, index, col);
    // rotL rotV
    collide_boundedPlane(ry, pos+rotU, rotL, rotV, rotU, index, col);
    // rotU rotV
    collide_boundedPlane(ry, pos+rotL, rotU, rotV, rotL, index, col);

    // negL negU
    collide_boundedPlane(ry, pos+negV, negL, negU, negV, index, col);
    // negL negV
    collide_boundedPlane(ry, pos+negU, negL, negV, negU, index, col);
    // negU negV
    collide_boundedPlane(ry, pos+negL, negU, negV, negL, index, col);
}

// parser helper functions
// get material value
int get_mat(int index) {
    return int(data[index*width+11]);
}

vec3 get_color(int index) {
    return vec3(data[index*width+13],data[index*width+14],data[index*width+15]);
}

float get_refidx(int index) {
    return data[index*width+12];
}

vec3 get_pos(int index) {
    return vec3(data[index*width+1],data[index*width+2],data[index*width+3]);
}

vec4 get_rot(int index) {
    return vec4(data[index*width+4],data[index*width+5],data[index*width+6],data[index*width+7]);
}

// accelerate processing of collisions by segmenting collisions into most reasonable collision
/* checks:
    * whether the ray intersects the bounding sphere of the shape 
    * maybe some other things later?
 */
bool accelerate(ray curRay, int index) {
    collision col;
    col.t = MAXT;
    col.obi = -1;

    switch (int(data[index*width])) {
        case 0: // sphere
            // actually wastes time if we check spheres twice
            return true;
        case 1: // box
            // also wastes time if we check boxes twice
            return true;
        case 2: // capsule
            // also wastes time if we check capsules twice
            return true;
        case 3: // mesh
            collide_sphere(
                curRay, 
                get_pos(index),
                data[index*width+9], // largest distance of mesh as defined by data
                get_rot(index),
                index,
                col
            );

            return col.obi != -1;
        default:
            return false;
    }
}

void processCol(ray curRay, inout collision col) {
    // determine collision for each shape in scene
    for (int k = 0; k < size; k ++) {
        if (accelerate(curRay, k)) {
            switch (int(data[k*width])) {
                case 0: // sphere
                    collide_sphere(
                        curRay, 
                        get_pos(k),
                        data[k*width+8],
                        get_rot(k),
                        k,
                        col
                    );
                    break;
                case 1: // box
                    collide_box(
                        curRay,
                        get_pos(k),
                        vec3(data[k*width+8],data[k*width+9],data[k*width+10]),
                        get_rot(k),
                        k,
                        col
                    );
                    break;
                case 2: // capsule
                    break;
                case 3: // mesh
                    // honestly not sure what I'm going to do about meshes, so I'm ignoring the problem for now
                    // having thousands of polygons to consider makes the program incredibly slow
                    /*for (int l = 0; l < 1000; l ++) {//int(data[k*width+8]/9); l ++) {
                        vec3 p = vec3(
                            data[size*width+9*l],
                            data[size*width+9*l+1],
                            data[size*width+9*l+2]
                        );
                        vec3 q = vec3(
                            data[size*width+9*l+3],
                            data[size*width+9*l+4],
                            data[size*width+9*l+5]
                        );
                        vec3 r = vec3(
                            data[size*width+9*l+6],
                            data[size*width+9*l+7],
                            data[size*width+9*l+8]
                        );
                            
                        collide_boundedTriangle(
                            curRay,
                            p,
                            q,
                            r,
                            k,
                            col
                        );
                    }*/
                    break;
                default:
                    break;
            }
        }
    }
}

bool refract2(vec3 v, vec3 n, float ni_over_nt, inout vec3 refracted) {
    vec3 uv = normalize(v);
    float dt = dot(uv, n);
    float disc = 1.0 - ni_over_nt * ni_over_nt * (1.0-dt*dt);
    if (disc > 0.0) {
        refracted = ni_over_nt * (uv - n*dt) - n*sqrt(disc);
        return true;
    } else {
        return false;
    }
}

float schlick(float csn, float idx) {
    float r0 = (1.0-idx) / (1.0+idx);
    r0 = r0*r0;
    return r0 + (1.0-r0)*pow(1.0-csn,5.0);
}

// parses provided data into shapes (given initial ray)
vec3 world(ray ry) {
    vec3 sumSamples = vec3(0);

    // each ray can be estimated MAXSAMPLES many times
    for (int i = 0; i < MAXSAMPLES; i ++) {
        vec3 curColor = vec3(0);
        vec3 attenuation = vec3(2);

        ray curRay;
        curRay.rd = ry.rd;
        curRay.ro = ry.ro;

        int bounceCount = 0;

        // each ray can bounce MAXBOUNCES many times
        for (int j = 0; j < MAXBOUNCES; j ++) {
            bounceCount ++;

            collision col;
            col.t = MAXT;
            col.obi = -1;

            processCol(curRay, col);
            
            // now col contains information about the closest object it intersects with
            if (col.obi == -1) { // flew into the void
                float t = curRay.rd.y;
                curColor = attenuation * ((1.0-t)*vec3(0.25,0.75,1.0)+t*vec3(0.25, 0.5, 0.75));
                break;
            }
            int switcher = get_mat(col.obi);
            switch (switcher) {
                case 0: // lambertian
                    curRay.ro = col.p;
                    curRay.rd = normalize(col.n + 0.9*randInUnitSphere());
                    attenuation *= 0.5*get_color(col.obi);
                    //curColor += abs(normalize(col.p) + vec3(cos(iTime), sin(iTime), cos(iTime)));//get_color(col.obi);
                    break;
                case 1: // metal
                    vec3 diff = randInUnitSphere();
                    curRay.ro = col.p;
                    vec3 rdT = reflect(normalize(curRay.rd), col.n);
                    curRay.rd = ((1.0-FUZZ)*rdT + FUZZ*diff) + FUZZ*(col.n + diff);
                    attenuation *= get_color(col.obi);
                    break;
                case 2: // glass
                    vec3 reflected = reflect(normalize(curRay.rd), col.n);
                    vec3 outward_normal;
                    float ni_over_nt;
                    vec3 refracted;
                    float reflect_prob = 1.0;
                    float csn;
                    float il = 1.0/length(curRay.rd);
                    float drdnor = dot(curRay.rd, col.n);
                    float idx = get_refidx(col.obi);
                    if (drdnor > 0.0) {
                        outward_normal = -col.n;
                        ni_over_nt = idx;
                        csn = ni_over_nt * drdnor * il;
                    } else {
                        outward_normal = col.n;
                        ni_over_nt = 1.0/idx;
                        csn = -ni_over_nt * drdnor * il;
                    }
                    if (refract2(curRay.rd, outward_normal, ni_over_nt, refracted)) {
                        reflect_prob = schlick(csn, idx);
                    }
                    if (randFloat() < reflect_prob) {
                        curRay.ro = col.p;
                        curRay.rd = reflected;
                    } else {
                        curRay.ro = col.p;
                        curRay.rd = refracted;
                    }
            }
        }

        sumSamples += sqrt(curColor) / bounceCount;
    }

    return sumSamples / MAXSAMPLES;
}

vec3 getC(vec2 uv, vec3 fd, vec3 upd, vec3 rtd, vec3 ro) {
    vec3 rd = normalize(fd+uv.x*rtd+uv.y*upd);
    
    ray ry;
    ry.ro = ro;
    ry.rd = rd;

    vec3 color = world(ry);
    
    return color;
}

void main() {
    float FOV = 0.5;
    vec2 uv = FOV*vec2(x, y);

    setSeed(uint(hash12(uv) * (y+1) * res.x * res.x + hash12(uv) * x));
    setSeed(randInt() + uint(iTime*10000));

    //vec3 ro = vec3(4*cos(iTime/10), 2, 4*sin(iTime/10));
    //vec3 ro = vec3(4, 2, 4);
    //vec3 fd = normalize(-ro);

    vec3 ro = cPos;
	vec3 fd = normalize(cRot);
	vec3 upd = normalize(vec3(-fd.y*fd.xz,length(fd.xz)).xzy);
	vec3 rtd = normalize(cross(fd,upd));
    
    vec3 C = vec3(0);

    //C += getC(uv+vec2( 1, 1)*0.25/(res/1.0), fd, upd, rtd, ro);
	//C += getC(uv+vec2( 1,-1)*0.25/(res/1.0), fd, upd, rtd, ro);
	//C += getC(uv+vec2(-1, 1)*0.25/(res/1.0), fd, upd, rtd, ro);
	//C += getC(uv+vec2(-1,-1)*0.25/(res/1.0), fd, upd, rtd, ro);

    //C /= 4;
    C += getC(uv, fd, upd, rtd, ro);

    gl_FragColor = vec4(C, 1);
}