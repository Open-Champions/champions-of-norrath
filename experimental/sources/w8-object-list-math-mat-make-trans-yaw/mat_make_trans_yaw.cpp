// Observed matMakeTransYaw__FP8Matrix34G6Point3s at 0x157668, extent 356.
// Writes a Matrix34 for a rotation about the vertical axis plus a translation.
// The Point3 argument arrives as a pointer to a caller copy and is copied again
// before the matrix is written. The angle is the low 16 bits of the third
// argument. With c and s the cosine and sine of the angle (the same polynomial
// as icos__Fi and isin__Fi, which the original has inlined here), the twelve
// floats at byte offsets 0..44 become
//   [ c  s  0 ]  [ -s  c  0 ]  [ 0  0  1 ]  and the translation x, y, z
// (floats 0,1,2 / 3,4,5 / 6,7,8 / 9,10,11). Returns nothing.
// Inferred: yaw is the angle about the vertical axis; the sign convention of s
// follows the observed -s at float 3.
#include "../w8-object-list-math-common/matrix_types.hpp"
#include "../w8-object-list-math-common/trig_core.hpp"

extern "C" void matMakeTransYaw__FP8Matrix34G6Point3s(Matrix34* matrix, Point3 translation, short yaw) {
    const float c = trig_core::evaluate(yaw);
    const float s = trig_core::evaluate(static_cast<short>(yaw - 16384));
    float* m = matrix->m;
    m[0] = c;
    m[1] = s;
    m[2] = 0.0f;
    m[3] = -s;
    m[4] = c;
    m[5] = 0.0f;
    m[6] = 0.0f;
    m[7] = 0.0f;
    m[8] = 1.0f;
    m[9] = translation.x;
    m[10] = translation.y;
    m[11] = translation.z;
}
