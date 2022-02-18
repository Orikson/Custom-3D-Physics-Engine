#include "vec4.h"

// Represents a 4 dimensional quaternion <x, y, z, w>
vec4::vec4(float x, float y, float z, float w) {
    this->x = x; this->y = y; this->z = z; this->w = w;
}
// Represents a 4 dimensional quaternion <f, f, f, f>
vec4::vec4(float f) {
    this->x = f; this->y = f; this->z = f; this->w = f;
}
// Copy constructor
vec4::vec4(const vec4& v) {
    this->x = v.x; this->y = v.y; this->z = v.z; this->w = v.w;
}
/** Define orientation quaternion about axis n rotated by theta radians
 * C = cos (theta/2)
 * S = sin (theta/2)
 * rot = vec4(C, X*S, Y*S, Z*S)
 */
vec4::vec4(const vec3& n, float theta) {
    vec3 nt = n; //vec3::norm(n);
    this->x = cosf(theta/2); this->y = nt.X()*sinf(theta/2); this->z = nt.Y()*sinf(theta/2); this->w = nt.Z()*sinf(theta/2);
}
// Represents a 4 dimensional quaternion <0, 0, 0, 0>
vec4::vec4() {
    this->x = 0; this->y = 0; this->z = 0; this->w = 0;
}

// Returns the x component of the quaternion
float vec4::X() const {
    return x;
}
// Returns the y component of the quaternion
float vec4::Y() const {
    return y;
}
// Returns the z component of the quaternion
float vec4::Z() const {
    return z;
}
// Returns the w component of the quaternion
float vec4::W() const {
    return w;
}

// Dot product (v1 dot v2)
float vec4::dot(const vec4& v1, const vec4& v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
}
// Projection (v1 projected on v2)
vec4 vec4::project(const vec4& v1, const vec4& v2) {
    float comp = dot(v2, v1)/mag(v2);
    return norm(v2) * comp;
}
// Normalize (|v|)
vec4 vec4::norm(const vec4& v) {
    return v / mag(v);
}

// Call a function on each component of the vector
vec4 vec4::callFunc_f1(const vec4& v, float (*func)(float)) {
    vec4 r;
    r.x = func(v.x);
    r.y = func(v.y);
    r.z = func(v.z);
    r.w = func(v.w);
    return r;
}
// Call a function on each component of the vector with an additional parameter
vec4 vec4::callFunc_f2(const vec4& v, float (*func)(float, float), float param) {
    vec4 r;
    r.x = func(v.x, param);
    r.y = func(v.y, param);
    r.z = func(v.z, param);
    r.w = func(v.w, param);
    return r;
}

// Invert the vector
vec4 vec4::invert(const vec4& v) {
    vec4 r;
    r.x = 1/v.x;
    r.y = 1/v.y;
    r.z = 1/v.z;
    r.w = 1/v.w;
    return r;
}
// Absolute value of the vector
vec4 vec4::abs(const vec4& v) {
    return callFunc_f1(v, std::abs);
}
// Float-capable modulus (previously defined in vec3.h)
/*float floatMod(float x, float y) {
    return x-y*floor(x/y);
}*/
// Result of each component mod the given value 
vec4 vec4::mod(const vec4& v, float f) {
    return callFunc_f2(v, floatMod, f);
}
// Round each component
vec4 vec4::round(const vec4& v) {
    return callFunc_f1(v, roundf);
}
// Floor each component
vec4 vec4::floor(const vec4& v) {
    return callFunc_f1(v, floorf);
}
// Ceiling each component
vec4 vec4::ceil(const vec4& v) {
    return callFunc_f1(v, ceilf);
}
// Take the sine of each component
vec4 vec4::sin(const vec4& v) {
    return callFunc_f1(v, sinf);
}
// Take the cosine of each component
vec4 vec4::cos(const vec4& v) {
    return callFunc_f1(v, cosf);
}
// Take the tangent of each component
vec4 vec4::tan(const vec4& v) {
    return callFunc_f1(v, tanf);
}
// Take the cosecant of each component
vec4 vec4::csc(const vec4& v) {
    return invert(sin(v));
}
// Take the secant of each component
vec4 vec4::sec(const vec4& v) {
    return invert(cos(v));
}
// Take the cotangent of each component
vec4 vec4::cot(const vec4& v) {
    return invert(tan(v));
}

float vec4::mag(const vec4& v) {
    return sqrt(dot(v,v));
}

vec4 vec4::operator+ (const vec4& v) const {
    vec4 r;
    r.x = x + v.x;
    r.y = y + v.y;
    r.z = z + v.z;
    r.w = w + v.w;
    return r;
}
vec4 vec4::operator- (const vec4& v) const {
    vec4 r;
    r.x = x - v.x;
    r.y = y - v.y;
    r.z = z - v.z;
    r.w = w - v.w;
    return r;
}
vec4 vec4::operator* (const vec4& v) const { // hamilton product
    float a1 = x; float a2 = v.x;
    float b1 = y; float b2 = v.y;
    float c1 = z; float c2 = v.z;
    float d1 = w; float d2 = v.w;

    return vec4(
        a1*a2-b1*b2-c1*c2-d1*d2,
        a1*b2+b1*a2+c1*d2-d1*c2,
        a1*c2-b1*d2+c1*a2+d1*b2,
        a1*d2+b1*c2-c1*b2+d1*a2
    );
}
vec4 vec4::operator/ (const vec4& v) const {
    vec4 r;
    r.x = x / v.x;
    r.y = y / v.y;
    r.z = z / v.z;
    r.w = w / v.w;
    return r;
}

vec4 vec4::operator+ (float f) const {
    vec4 r;
    r.x = x + f;
    r.y = y + f;
    r.z = z + f;
    r.w = w + f;
    return r;
}
vec4 vec4::operator- (float f) const {
    vec4 r;
    r.x = x - f;
    r.y = y - f;
    r.z = z - f;
    r.w = w - f;
    return r;
}
vec4 vec4::operator* (float f) const {
    vec4 r;
    r.x = x * f;
    r.y = y * f;
    r.z = z * f;
    r.w = w * f;
    return r;
}
vec4 vec4::operator/ (float f) const {
    vec4 r;
    r.x = x / f;
    r.y = y / f;
    r.z = z / f;
    r.w = w / f;
    return r;
}

void vec4::operator= (const vec4& v) {
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
}
void vec4::operator+= (const vec4& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
}
void vec4::operator-= (const vec4& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
}
void vec4::operator*= (const vec4& v) {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
}
void vec4::operator/= (const vec4& v) {
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
}

void vec4::operator+= (float f) {
    x += f;
    y += f;
    z += f;
    w += f;
}
void vec4::operator-= (float f) {
    x -= f;
    y -= f;
    z -= f;
    w -= f;
}
void vec4::operator*= (float f) {
    x *= f;
    y *= f;
    z *= f;
    w *= f;
}
void vec4::operator/= (float f) {
    x /= f;
    y /= f;
    z /= f;
    w /= f;
}