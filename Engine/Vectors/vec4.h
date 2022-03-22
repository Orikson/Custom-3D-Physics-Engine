#ifndef _VEC4_H
#define _VEC4_H

#include "../../common.h"

class vec4 {
    public:
        vec4(float x, float y, float z, float w);
        vec4(float f);
        vec4(const vec4& v);
        vec4(const vec3& n, float theta);
        vec4();

        float X() const;
        float Y() const;
        float Z() const;
        float W() const;

        static float dot(const vec4& v1, const vec4& v2);
        static vec4 project(const vec4& v1, const vec4& v2);
        static vec4 norm(const vec4& v);
        
        static vec4 callFunc_f1(const vec4& v, float (*func)(float));
        static vec4 callFunc_f2(const vec4& v, float (*func)(float, float), float param);

        static vec4 invert(const vec4& v);
        static vec4 abs(const vec4& v);
        static vec4 fract(const vec4& v);
        static vec4 mod(const vec4& v, float f);
        static vec4 round(const vec4& v);
        static vec4 floor(const vec4& v);
        static vec4 ceil(const vec4& v);
        static vec4 sin(const vec4& v);
        static vec4 cos(const vec4& v);
        static vec4 tan(const vec4& v);
        static vec4 csc(const vec4& v);
        static vec4 sec(const vec4& v);
        static vec4 cot(const vec4& v);

        static float mag(const vec4& v);

        vec4 operator+ (const vec4& v) const;
        vec4 operator- (const vec4& v) const;
        vec4 operator* (const vec4& v) const;
        vec4 operator/ (const vec4& v) const;
        
        vec4 operator+ (float f) const;
        vec4 operator- (float f) const;
        vec4 operator* (float f) const;
        vec4 operator/ (float f) const;
        
        void operator= (const vec4& v);
        void operator+= (const vec4& v);
        void operator-= (const vec4& v);
        void operator*= (const vec4& v);
        void operator/= (const vec4& v);

        void operator+= (float f);
        void operator-= (float f);
        void operator*= (float f);
        void operator/= (float f);
    
    private:
        float x, y, z, w;
        
};

//#include "vec4.cpp"

#endif