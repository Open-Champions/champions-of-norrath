// Observed matUntransform__FP6Point3P8Matrix44T0 at 0x00159a00, extent 220.
// (output, matrix, input) -> float. The input vector is copied to the stack first,
// so the output may be the input. The sixteen floats m are read as four rows of
// four; each sum is taken left to right with the input components (x, y, z):
//   out.x = m0*x + m1*y + m2*z  + m3
//   out.y = m4*x + m5*y + m6*z  + m7
//   out.z = m8*x + m9*y + m10*z + m11
//   result = m3*x + m7*y + m11*z + m15   (returned in a float register)
// The returned sum reads byte offsets 12, 28, 44 and 60, the fourth element of
// each of the four rows.
// Inferred: the returned float is the fourth component of the same product.
// Unknown: the declared return type (the mangled name omits it; the float
// register at return shows a float is produced), and whether any caller reads it.
#include "../w9-render-math-b-common/math_types.hpp"

extern "C" float matUntransform__FP6Point3P8Matrix44T0(Point3* out, const Matrix44* matrix, const Point3* in) {
    const Point3 v = *in;
    const float* m = matrix->m;
    out->x = m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3];
    out->y = m[4] * v.x + m[5] * v.y + m[6] * v.z + m[7];
    out->z = m[8] * v.x + m[9] * v.y + m[10] * v.z + m[11];
    return m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15];
}
