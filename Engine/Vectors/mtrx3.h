// 3x3 matrix class
#ifndef _MTRX3_H
#define _MTRX3_H

#include "../../common.h"

class mtrx3 {
    public:
        // defined by column vectors
        mtrx3(vec3 a, vec3 b, vec3 c);
        mtrx3(const mtrx3& m);
        // defaults to identity vector
        mtrx3();

        // define row vectors (for transpose)
        void defT();

        // transpose
        mtrx3 t();
        // determinant
        float det();
        // trace
        float trace();
        // inverse
        mtrx3 inverse();
        // adjugate
        mtrx3 adj();

        // 3x3 identity matrix
        static mtrx3 I;

        mtrx3 operator+ (const mtrx3& m) const;
        mtrx3 operator- (const mtrx3& m) const;
        mtrx3 operator* (const mtrx3& m) const;
        mtrx3 operator/ (const mtrx3& m) const;

        vec3 operator* (const vec3& v) const;
        
        mtrx3 operator+ (float f) const;
        mtrx3 operator- (float f) const;
        mtrx3 operator* (float f) const;
        mtrx3 operator/ (float f) const;

        void operator= (const mtrx3& m);
        void operator+= (const mtrx3& m);
        void operator-= (const mtrx3& m);
        void operator*= (const mtrx3& m);
        void operator/= (const mtrx3& m);

        void operator+= (float f);
        void operator-= (float f);
        void operator*= (float f);
        void operator/= (float f);
    
    private:
        // column vectors
        vec3 a, b, c;
        // row vectors (accessible for transpose)
        vec3 at, bt, ct;
};

// 3x3 Identity matrix
mtrx3 mtrx3::I = mtrx3(vec3(1, 0, 0), 
                       vec3(0, 1, 0), 
                       vec3(0, 0, 1));

#endif