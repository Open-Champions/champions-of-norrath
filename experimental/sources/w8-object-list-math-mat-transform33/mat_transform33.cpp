// Observed matTransform__FP6Point3P8Matrix33T0 at 0x159880, extent 160.
// (output, matrix, input) -> output. The input vector is copied to the stack
// first, so the output may be the input. Each output component is a left-to-right
// sum of three products from one column of the nine floats, taken at byte
// offsets 0/12/24, 4/16/28 and 8/20/32:
//   out.x = m0*v.x + m3*v.y + m6*v.z
//   out.y = m1*v.x + m4*v.y + m7*v.z
//   out.z = m2*v.x + m5*v.y + m8*v.z
// Returns the output pointer. Inferred: the vector is multiplied as a row.
#include "../w8-object-list-math-common/matrix_types.hpp"

extern "C" Point3* matTransform__FP6Point3P8Matrix33T0(Point3* out, const Matrix33* matrix, const Point3* in) {
    const Point3 v = *in;
    const float* m = matrix->m;
    out->x = m[0] * v.x + m[3] * v.y + m[6] * v.z;
    out->y = m[1] * v.x + m[4] * v.y + m[7] * v.z;
    out->z = m[2] * v.x + m[5] * v.y + m[8] * v.z;
    return out;
}
