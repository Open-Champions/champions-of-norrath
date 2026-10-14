// Observed matUntransform__FP6Point3P8Matrix33T0 at 0x001597e0, extent 160.
// (output, matrix, input) -> output. The input vector is copied to the stack first,
// so the output may be the input. Each output component is a left-to-right sum of
// three products taken along one row of the nine floats (byte offsets 0/4/8,
// 12/16/20 and 24/28/32):
//   out.x = m0*v.x + m1*v.y + m2*v.z
//   out.y = m3*v.x + m4*v.y + m5*v.z
//   out.z = m6*v.x + m7*v.y + m8*v.z
// Returns the output pointer. This is the transpose of the multiply in
// matTransform__FP6Point3P8Matrix33T0 (which reads columns).
// Inferred: for a pure rotation this undoes matTransform; the function does not
// test that the matrix is a rotation.
#include "../w9-render-math-b-common/math_types.hpp"

extern "C" Point3* matUntransform__FP6Point3P8Matrix33T0(Point3* out, const Matrix33* matrix, const Point3* in) {
    const Point3 v = *in;
    const float* m = matrix->m;
    out->x = m[0] * v.x + m[1] * v.y + m[2] * v.z;
    out->y = m[3] * v.x + m[4] * v.y + m[5] * v.z;
    out->z = m[6] * v.x + m[7] * v.y + m[8] * v.z;
    return out;
}
