#ifndef _VEC3_H
#define _VEC3_H

#include "../../common.h"

class vec3 {
    public:
        vec3(float x, float y, float z);
        vec3(float f);
        vec3(const vec3& v);
        vec3();

        float X() const;
        float Y() const;
        float Z() const;

        static float dot(const vec3& v1, const vec3& v2);
        static vec3 cross(const vec3& v1, const vec3& v2);
        static vec3 project(const vec3& v1, const vec3& v2);
        static vec3 norm(const vec3& v);
        
        static vec3 callFunc_f1(const vec3& v, float (*func)(float));
        static vec3 callFunc_f2(const vec3& v, float (*func)(float, float), float param);

        static vec3 invert(const vec3& v);
        static vec3 abs(const vec3& v);
        static vec3 fract(const vec3& v);
        static vec3 mod(const vec3& v, float f);
        static vec3 round(const vec3& v);
        static vec3 floor(const vec3& v);
        static vec3 ceil(const vec3& v);
        static vec3 sin(const vec3& v);
        static vec3 cos(const vec3& v);
        static vec3 tan(const vec3& v);
        static vec3 csc(const vec3& v);
        static vec3 sec(const vec3& v);
        static vec3 cot(const vec3& v);

        static float mag(const vec3& v);

        vec3 operator+ (const vec3& v) const;
        vec3 operator- (const vec3& v) const;
        vec3 operator* (const vec3& v) const;
        vec3 operator/ (const vec3& v) const;
        
        vec3 operator+ (float f) const;
        vec3 operator- (float f) const;
        vec3 operator* (float f) const;
        vec3 operator/ (float f) const;
        
        void operator= (const vec3& v);
        void operator+= (const vec3& v);
        void operator-= (const vec3& v);
        void operator*= (const vec3& v);
        void operator/= (const vec3& v);

        void operator+= (float f);
        void operator-= (float f);
        void operator*= (float f);
        void operator/= (float f);
    
    private:
        float x, y, z;
        
};

#include "vec3.cpp"

#endif