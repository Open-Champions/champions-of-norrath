// Observed matTransform34Vec__FP6Point3P8Matrix34T0 at 0x159b98, extent 160.
// The same arithmetic as matTransform__FP6Point3P8Matrix33T0 on the first nine
// floats of a Matrix34 (offsets 0/12/24, 4/16/28, 8/20/32); the translation
// floats at the end are not read, so this transforms a direction. The input is
// copied before the output is written, so the output may be the input. Returns
// the output pointer.
#include "../w8-object-list-math-common/matrix_types.hpp"

extern "C" Point3* matTransform34Vec__FP6Point3P8Matrix34T0(Point3* out, const Matrix34* matrix, const Point3* in) {
    const Point3 v = *in;
    const float* m = matrix->m;
    out->x = m[0] * v.x + m[3] * v.y + m[6] * v.z;
    out->y = m[1] * v.x + m[4] * v.y + m[7] * v.z;
    out->z = m[2] * v.x + m[5] * v.y + m[8] * v.z;
    return out;
}
