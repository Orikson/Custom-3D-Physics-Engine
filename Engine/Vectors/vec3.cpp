#include "vec3.h"

// prints vec3's
void vec3::printv3(const vec3& v) {
    cout << v.X() << " " << v.Y() << " " << v.Z();
}

// Represents a 3 dimensional vector <x, y, z>
vec3::vec3(float x, float y, float z) {
    this->x = x; this->y = y; this->z = z;
}
// Represents a 3 dimensional vector <f, f, f>
vec3::vec3(float f) {
    this->x = f; this->y = f; this->z = f;
}
// Copy constructor
vec3::vec3(const vec3& v) {
    this->x = v.x; this->y = v.y; this->z = v.z;
}
// Represents a 3 dimensional vector <0, 0, 0>
vec3::vec3() {
    this->x = 0; this->y = 0; this->z = 0;
}

// Return the x component of the vector
float vec3::X() const {
    return x;
}
// Return the y component of the vector
float vec3::Y() const {
    return y;
}
// Return the z component of the vector
float vec3::Z() const {
    return z;
}

// Shortest distance from point p to line segment defined by v1 and v2
vec3 vec3::shortestDistanceToLineSegment(const vec3& p, const vec3& v1, const vec3& v2) {
    vec3 ld = v2 - v1;
    vec3 pd = p - v1;
    vec3 ldd = v1 - v2;
    vec3 pdd = p - v2;
    vec3 pdld = vec3::project(pd, ld);
    vec3 orth = pdld - pd;
    vec3 l1 = v1 - p;
    float m1 = vec3::dot(l1, l1);
    vec3 l2 = v2 - p;
    float m2 = vec3::dot(l2, l2);
    
    if (dot(ld, pdd) < 0 && dot(ldd, pd) < 0) {
        return orth;
    } else if (m1 < m2) {
        return l1;
    } else {
        return l2;
    }
}

// Rotate v by quaternion rot
vec3 vec3::rotate(const vec3& v, const vec4& rot) {
    vec4 quantN = vec4(0, v.X(), v.Y(), v.Z());
    vec4 rotPrime = vec4(rot.X(), -rot.Y(), -rot.Z(), -rot.W());

    vec4 quantNPrime = (rot*quantN)*rotPrime;

    return vec3(quantNPrime.Y(), quantNPrime.Z(), quantNPrime.W());
}


// Dot product (v1 dot v2)
float vec3::dot(const vec3& v1, const vec3& v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}
// Cross product (v1 cross v2)
vec3 vec3::cross(const vec3& v1, const vec3& v2) {
    vec3 r;
    r.x = v1.y*v2.z - v1.z*v2.y;
    r.y = v1.z*v2.x - v1.x*v2.z;
    r.z = v1.x*v2.y - v1.y*v2.x;
    return r;
}
// Projection (v1 projected onto v2)
vec3 vec3::project(const vec3& v1, const vec3& v2) {
    float comp = dot(v2, v1)/mag(v2);
    return norm(v2) * comp;
}
// Normalize (|v|)
vec3 vec3::norm(const vec3& v) {
    return v / mag(v);
}

// Call single float function on each component of vector (e.g. <x,y,z> -> <f(x),f(y),f(z)>)
vec3 vec3::callFunc_f1(const vec3& v, float (*func)(float)) {
    vec3 r;
    r.x = func(v.x);
    r.y = func(v.y);
    r.z = func(v.z);
    return r;
}
// Call double float function on each component of vector (e.g. <x,y,z> -> <f(x,c),f(y,c),f(z,c)>)
vec3 vec3::callFunc_f2(const vec3& v, float (*func)(float, float), float param) {
    vec3 r;
    r.x = func(v.x, param);
    r.y = func(v.y, param);
    r.z = func(v.z, param);
    return r;
}

// Invert vector
vec3 vec3::invert(const vec3& v) {
    vec3 r;
    r.x = 1/v.x;
    r.y = 1/v.y;
    r.z = 1/v.z;
    return r;
}
// Absolute value of each component
vec3 vec3::abs(const vec3& v) {
    return callFunc_f1(v, std::abs);
}
// Fractional part of each component
vec3 vec3::fract(const vec3& v) {
    vec3 r;
    double itemp;
    r.x = modf(v.x, &itemp);
    r.y = modf(v.y, &itemp);
    r.z = modf(v.z, &itemp);
    return r;
}
// Float-capable modulus
float floatMod(float x, float y) {
    return x-y*floor(x/y);
}
// Result of each component mod the given value 
vec3 vec3::mod(const vec3& v, float f) {
    return callFunc_f2(v, floatMod, f);
}
// Round each component
vec3 vec3::round(const vec3& v) {
    return callFunc_f1(v, roundf);
}
// Floor each component
vec3 vec3::floor(const vec3& v) {
    return callFunc_f1(v, floorf);
}
// Ceiling each component
vec3 vec3::ceil(const vec3& v) {
    return callFunc_f1(v, ceilf);
}
// Take the sine of each component
vec3 vec3::sin(const vec3& v) {
    return callFunc_f1(v, sinf);
}
// Take the cosine of each component
vec3 vec3::cos(const vec3& v) {
    return callFunc_f1(v, cosf);
}
// Take the tangent of each component
vec3 vec3::tan(const vec3& v) {
    return callFunc_f1(v, tanf);
}
// Take the cosecant of each component
vec3 vec3::csc(const vec3& v) {
    return invert(sin(v));
}
// Take the secant of each component
vec3 vec3::sec(const vec3& v) {
    return invert(cos(v));
}
// Take the cotangent of each component
vec3 vec3::cot(const vec3& v) {
    return invert(tan(v));
}

// Take the magnitude of the vector
float vec3::mag(const vec3& v) {
    return sqrt(dot(v,v));
}

vec3 vec3::operator+ (const vec3& v) const {
    vec3 r;
    r.x = x + v.x;
    r.y = y + v.y;
    r.z = z + v.z;
    return r;
}
vec3 vec3::operator- (const vec3& v) const {
    vec3 r;
    r.x = x - v.x;
    r.y = y - v.y;
    r.z = z - v.z;
    return r;
}
vec3 vec3::operator* (const vec3& v) const {
    vec3 r;
    r.x = x * v.x;
    r.y = y * v.y;
    r.z = z * v.z;
    return r;
}
vec3 vec3::operator/ (const vec3& v) const {
    vec3 r;
    r.x = x / v.x;
    r.y = y / v.y;
    r.z = z / v.z;
    return r;
}

vec3 vec3::operator+ (float f) const {
    vec3 r;
    r.x = x + f;
    r.y = y + f;
    r.z = z + f;
    return r;
}
vec3 vec3::operator- (float f) const {
    vec3 r;
    r.x = x - f;
    r.y = y - f;
    r.z = z - f;
    return r;
}
vec3 vec3::operator* (float f) const {
    vec3 r;
    r.x = x * f;
    r.y = y * f;
    r.z = z * f;
    return r;
}
vec3 vec3::operator/ (float f) const {
    vec3 r;
    r.x = x / f;
    r.y = y / f;
    r.z = z / f;
    return r;
}

void vec3::operator= (const vec3& v) {
    x = v.x;
    y = v.y;
    z = v.z;
}
void vec3::operator+= (const vec3& v) {
    x += v.x;
    y += v.y;
    z += v.z;
}
void vec3::operator-= (const vec3& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
}
void vec3::operator*= (const vec3& v) {
    x *= v.x;
    y *= v.y;
    z *= v.z;
}
void vec3::operator/= (const vec3& v) {
    x /= v.x;
    y /= v.y;
    z /= v.z;
}

void vec3::operator+= (float f) {
    x += f;
    y += f;
    z += f;
}
void vec3::operator-= (float f) {
    x -= f;
    y -= f;
    z -= f;
}
void vec3::operator*= (float f) {
    x *= f;
    y *= f;
    z *= f;
}
void vec3::operator/= (float f) {
    x /= f;
    y /= f;
    z /= f;
}