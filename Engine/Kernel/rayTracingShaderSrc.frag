#version 430

// float coordinates defined by vertex shader (between -1 and 1)
varying float x, y, z;
uniform int iFrame;
uniform float iTime;
uniform vec3 cPos;
uniform vec3 cRot;

const float MAXD = 100.0;
const float MAXT = 100.0;

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

}

// ray, plane normal/point on plane
void collide_plane(ray ry, vec3 n, vec3 p, int index, inout collision col) {
    // if parallel approx 0 (for error < 0.001)
    if (abs(dot(n, ry.rd)) > 0.001) {
        float tempT = dot(p-ry.ro, n)/dot(ry.rd, n);
        
        if (tempT < col.t && tempT > 0) {
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

        if (tempT < col.t && tempT > 0 && dot(a, v) < dot(v, v) && dot(a, u) < dot(u, u) && -dot(a, v) < dot(v, v) && -dot(a, u) < dot(u, u)) {
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

vec3 getC(vec2 uv, vec3 fd, vec3 upd, vec3 rtd, vec3 ro) {
    vec3 rd = normalize(fd+uv.x*rtd+uv.y*upd);
    
    collision col;
    col.t = MAXT;
    ray ry;
    ry.ro = ro;
    ry.rd = rd;

    collide_box(
        ry, 
        vec3(0, 0, 0), 
        vec3(1, 1, 1), 
        quaternion(vec3(0, 0, 1), 0),
        0,
        col
    );

    return col.n;
}

void main() {
    float FOV = 0.5;
    vec2 uv = FOV*vec2(x, y);

    setSeed(uint(hash12(uv) * (y+1) * res.x * res.x + hash12(uv) * x));
    setSeed(randInt());// + uint(iTime*10000));

    vec3 ro = vec3(4*cos(iTime/10), 2, 4*sin(iTime/10));
    vec3 fd = normalize(-ro);

    //vec3 ro = cPos;
	//vec3 fd = normalize(cRot);
	vec3 upd = normalize(vec3(-fd.y*fd.xz,length(fd.xz)).xzy);
	vec3 rtd = normalize(cross(fd,upd));
    
    vec3 C = vec3(0);

    C += getC(uv+vec2( 1, 1)*0.25/(res/1.0), fd, upd, rtd, ro);
	C += getC(uv+vec2( 1,-1)*0.25/(res/1.0), fd, upd, rtd, ro);
	C += getC(uv+vec2(-1, 1)*0.25/(res/1.0), fd, upd, rtd, ro);
	C += getC(uv+vec2(-1,-1)*0.25/(res/1.0), fd, upd, rtd, ro);

    C /= 4;

    gl_FragColor = vec4(C, 1);
}