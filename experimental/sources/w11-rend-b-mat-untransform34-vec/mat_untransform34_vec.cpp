// Observed matUntransform34Vec__FP6Point3P8Matrix34T0 at 0x00159c38, extent 160.
// Arguments: output Point3, Matrix34 (twelve floats), input Point3. The input is
// copied to the stack first, so the output may alias the input. Only the first nine
// floats (the three rows of three) are read; the translation row at +36 is not.
//   out.x = m[0]*v.x + m[1]*v.y + m[2]*v.z
//   out.y = m[3]*v.x + m[4]*v.y + m[5]*v.z
//   out.z = m[6]*v.x + m[7]*v.y + m[8]*v.z
// Each sum is accumulated left to right. Returns the output pointer. It is the
// Matrix34 sibling of matUntransform__FP6Point3P8Matrix33T0 and the same rows.
// Inferred: the name says this applies the matrix without translation; the reading of
// the rows as an inverse rotation depends on how callers build the matrix.
#include "../w9-render-math-b-common/math_types.hpp"

extern "C" Point3* matUntransform34Vec__FP6Point3P8Matrix34T0(Point3* out, const Matrix34* matrix, const Point3* in) {
    const Point3 v = *in;
    const float* m = matrix->m;
    out->x = m[0] * v.x + m[1] * v.y + m[2] * v.z;
    out->y = m[3] * v.x + m[4] * v.y + m[5] * v.z;
    out->z = m[6] * v.x + m[7] * v.y + m[8] * v.z;
    return out;
}
