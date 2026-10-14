// Observed matMul__FP8Matrix44P8Matrix34T0 at 0x00156348, extent 368. (out, a, b)
// -> out. In the mangled name T0 repeats the first parameter's type, so out and b
// are Matrix44 (sixteen floats, row stride 16 bytes) and a is Matrix34 (twelve
// floats, row stride 12 bytes). The sixteen results are built in a stack
// temporary and copied to out in 64 bytes at the end, so out may be b.
//   rows i = 0..2, columns j = 0..3:
//     t[4i+j] = (a[3i]*b[j] + a[3i+1]*b[4+j]) + a[3i+2]*b[8+j]
//   row 3, j = 0..3:
//     t[12+j] = ((a[9]*b[j] + a[10]*b[4+j]) + a[11]*b[8+j]) + b[12+j]
// Each sum is evaluated left to right as written. All sixteen floats of b are
// read (column 3 is an ordinary column). Returns the out pointer.
// Inferred: the product of an affine transform a and a full matrix b.
#include "../w9-render-math-b-common/math_types.hpp"

extern "C" Matrix44* matMul__FP8Matrix44P8Matrix34T0(Matrix44* out, const Matrix34* a, const Matrix44* b) {
    float t[16];
    for (int i = 0; i < 3; ++i) {
        const float* row = a->m + 3 * i;
        for (int j = 0; j < 4; ++j) {
            t[4 * i + j] = (row[0] * b->m[j] + row[1] * b->m[4 + j]) + row[2] * b->m[8 + j];
        }
    }
    const float* row = a->m + 9;
    for (int j = 0; j < 4; ++j) {
        t[12 + j] = ((row[0] * b->m[j] + row[1] * b->m[4 + j]) + row[2] * b->m[8 + j]) + b->m[12 + j];
    }
    for (int k = 0; k < 16; ++k) out->m[k] = t[k];
    return out;
}
