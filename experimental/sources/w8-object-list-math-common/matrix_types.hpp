#pragma once
#include "object_types.hpp"  // Point3
// Float matrices as the mangled names imply (Matrix33, Matrix34): nine and twelve
// consecutive floats. The routines that read them below address the elements by
// byte offset 4 * index.
struct Matrix33 {
    float m[9];
};
struct Matrix34 {
    float m[12];
};
struct Quat {
    float x, y, z, w;
};
