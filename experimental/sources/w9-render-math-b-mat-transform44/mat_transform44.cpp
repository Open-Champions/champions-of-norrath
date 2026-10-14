// Observed matTransform__FP6Point3P8Matrix44T0 at 0x00159920, extent 220.
// (output, matrix, input) -> float. The input vector is copied to the stack first,
// so the output may be the input. For each of four columns j (float offsets
// j, 4+j, 8+j, 12+j of the sixteen floats) the sum is evaluated left to right as
//   ((m[j]*v.x + m[4+j]*v.y) + m[8+j]*v.z) + m[12+j]
// i.e. the input is treated as the point (x, y, z, 1) multiplied as a row.
// Columns 0, 1 and 2 are stored to out.x, out.y and out.z. Column 3 is not stored;
// its value is the function's float result (left in the float return register).
// Inferred: that fourth value is the homogeneous w of the transformed point.
// Unknown: whether any caller reads it.
#include "../w9-render-math-b-common/math_types.hpp"

extern "C" float matTransform__FP6Point3P8Matrix44T0(Point3* out, const Matrix44* matrix, const Point3* in) {
    const Point3 v = *in;
    const float* m = matrix->m;
    out->x = ((m[0] * v.x + m[4] * v.y) + m[8] * v.z) + m[12];
    out->y = ((m[1] * v.x + m[5] * v.y) + m[9] * v.z) + m[13];
    out->z = ((m[2] * v.x + m[6] * v.y) + m[10] * v.z) + m[14];
    return ((m[3] * v.x + m[7] * v.y) + m[11] * v.z) + m[15];
}
