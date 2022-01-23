#include "mtrx3.h"

// defined by column vectors
mtrx3::mtrx3(vec3 a, vec3 b, vec3 c) {
    this->a = a; this->b = b; this->c = c;
    defT();
}
mtrx3::mtrx3(const mtrx3& m) {
    this->a = m.a; this->b = m.b; this->c = m.c;
    defT();
}
// defaults to identity vector
mtrx3::mtrx3() {
    this->a = mtrx3::I.a; this->b = mtrx3::I.b; this->c = mtrx3::I.c;
    defT();
}

void mtrx3::defT() {
    this->at = vec3(a.X(), b.X(), c.X());
    this->bt = vec3(a.Y(), b.Y(), c.Y());
    this->ct = vec3(a.Z(), b.Z(), c.Z());
}

// transpose
mtrx3 mtrx3::t() {
    return mtrx3(at, bt, ct);
}
// determinant
float mtrx3::det() {
    
}
// trace
float mtrx3::trace() {

}
// inverse
mtrx3 mtrx3::inverse() {
    if (this->det() == 0) {
        throw "Un-invertible matrix (determinant is 0)";
    }
    return this->adj() * (1/this->det());
}
// adjugate
mtrx3 mtrx3::adj() {
    return mtrx3(
        vec3(b.Y()*c.Z() - c.Y()*b.Z(), -(b.X()*c.Z() - c.X()*b.Z()), b.X()*c.Y() - c.X()*b.Y()),
        vec3(-(a.Y()*c.Z() - c.Y()*a.Z()), a.X()*c.Z() - c.X()*a.Z(), -(a.X()*c.Y() - c.X()*a.Y())),
        vec3(a.Y()*b.Z() - b.Y()*a.Z(), -(a.X()*b.Z() - b.X()*a.Z()), a.X()*b.Y() - b.X()*a.Y())
    );
}

mtrx3 mtrx3::operator+ (const mtrx3& m) const {
    return mtrx3(a + m.a, b + m.b, c + m.c);
}
mtrx3 mtrx3::operator- (const mtrx3& m) const {
    return mtrx3(a - m.a, b - m.b, c - m.c);
}
mtrx3 mtrx3::operator* (const mtrx3& m) const {
    return mtrx3(vec3(vec3::dot(at,m.a), vec3::dot(bt,m.a), vec3::dot(ct,m.a)),
                 vec3(vec3::dot(at,m.b), vec3::dot(bt,m.b), vec3::dot(ct,m.b)),
                 vec3(vec3::dot(at,m.c), vec3::dot(bt,m.c), vec3::dot(ct,m.c)));
}
mtrx3 mtrx3::operator/ (const mtrx3& m) const {
    throw "?";
}

vec3 mtrx3::operator* (const vec3& v) const {
    return vec3(vec3::dot(at, v), vec3::dot(bt, v), vec3::dot(ct, v));
}

mtrx3 mtrx3::operator+ (float f) const {

}
mtrx3 mtrx3::operator- (float f) const {

}
mtrx3 mtrx3::operator* (float f) const {

}
mtrx3 mtrx3::operator/ (float f) const {

}

void mtrx3::operator= (const mtrx3& m) {

}
void mtrx3::operator+= (const mtrx3& m) {

}
void mtrx3::operator-= (const mtrx3& m) {

}
void mtrx3::operator*= (const mtrx3& m) {

}
void mtrx3::operator/= (const mtrx3& m) {

}

void mtrx3::operator+= (float f) {

}
void mtrx3::operator-= (float f) {

}
void mtrx3::operator*= (float f) {

}
void mtrx3::operator/= (float f) {
    
}