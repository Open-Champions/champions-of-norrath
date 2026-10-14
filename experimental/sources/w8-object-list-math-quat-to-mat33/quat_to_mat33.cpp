// Observed quatToMat__FP8Matrix33P4Quat at 0x15a2b0, extent 208.
// Builds a 3x3 rotation matrix from a quaternion stored x, y, z, w. The scale is
// s = 2 / (x*x + y*y + z*z + w*w), the squared length summed left to right; there
// is no zero test, so a zero quaternion divides by zero. With a = x*s, b = y*s,
// c = z*s the terms are products such as x*a, and the nine floats written are
//   m0 = 1 - (yy + zz)  m1 = xy + wz        m2 = xz - wy
//   m3 = xy - wz        m4 = 1 - (xx + zz)  m5 = yz + wx
//   m6 = xz + wy        m7 = yz - wx        m8 = 1 - (xx + yy)
// where each product carries the factor s (xx = x*(x*s), xy = x*(y*s), wz = w*(z*s),
// and so on). Returns nothing. Inferred: unit quaternions give orthonormal
// matrices; unknown which handedness the engine intends.
#include "../w8-object-list-math-common/matrix_types.hpp"

extern "C" void quatToMat__FP8Matrix33P4Quat(Matrix33* matrix, const Quat* quat) {
    const float x = quat->x, y = quat->y, z = quat->z, w = quat->w;
    const float scale = 2.0f / (x * x + y * y + z * z + w * w);
    const float sx = x * scale, sy = y * scale, sz = z * scale;
    const float xx = x * sx, yy = y * sy, zz = z * sz;
    const float xy = x * sy, xz = x * sz, yz = y * sz;
    const float wx = w * sx, wy = w * sy, wz = w * sz;
    float* m = matrix->m;
    m[0] = 1.0f - (yy + zz);
    m[1] = xy + wz;
    m[2] = xz - wy;
    m[3] = xy - wz;
    m[4] = 1.0f - (xx + zz);
    m[5] = yz + wx;
    m[6] = xz + wy;
    m[7] = yz - wx;
    m[8] = 1.0f - (xx + yy);
}
