#ifndef _VECTORS_H
#define _VECTORS_H

#include "../../common.h"

class Vector {
    public:
        template <typename... Types> Vector(int size, Types... contents);
        Vector(const Vector &vector);
        Vector();
        ~Vector();

        // used to store contents of the vector
        vector<double> vectorContents;

        // size of vector
        int vectorSize;

        // magnitude of vector
        double magnitude;

        // variadic add values
        template <typename Type, typename... Types> void addvals(Type item, Types... contents);
        void addvals();

        

};

#include "vectors.cpp"

#endif