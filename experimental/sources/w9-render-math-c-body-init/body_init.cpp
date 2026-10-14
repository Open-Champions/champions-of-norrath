// Observed: bodyInit__FP5_bodyfffff at 0x0015bcd0, 552 bytes, is the function behind the
// placeholder norrath_body_init (called by the Push Physics Prop constructor with the
// object address + 80 and five floats). This file defines that placeholder name.
// Arguments: the body, then five floats p0..p4 (the names are not known).
// Effects, by byte offset into the body:
//   - 152 bytes from offset 0 are cleared first.
//   - 0: 1/p0   4: 12 / (p1*p0 * (p3*p3 + p4*p4))   8: 12 / (p1*p0 * (p2*p2 + p4*p4))
//     12: 12 / (p1*p0 * (p2*p2 + p3*p3))
//   - 28..40 is a quaternion (x, y, z, w) = (0, 0, 1.0, r): offset 36 gets 1.0 and offset 40
//     gets r = (state >> 16) * 2^-16 from the global eRandState, which is advanced as
//     state = state * 1664525 + 1013904223 (0x19660d, 0x3c6ef35f), 32-bit.
//     The quaternion is then normalized twice (quatNormalize__FP4Quat).
//   - offsets 56, 60, 64 are set to zero.
//   - offsets 68..100 receive the nine floats of the transpose of quatToMat__FP8Matrix33P4Quat
//     for that quaternion (offset 68 + 4*k: 0 gets 1-(yy+zz), 4 gets xy-wz, 8 gets xz+wy,
//     12 gets xy+wz, 16 gets 1-(xx+zz), 20 gets yz-wx, 24 gets xz-wy, 28 gets yz+wx,
//     32 gets 1-(xx+yy), with the same 2/n scaling).
//   - finally matUntransform__FP6Point3P8Matrix33T0 is called with (body+140, body+104,
//     body+56).
// Inferred: a rigid-body setup with inverse mass and inverse inertia terms; the random
// value perturbs the starting orientation. Unknown: the field names and the contents of
// the Matrix33 at offset 104 (zero after the clear).
extern int eRandState;
extern "C" void* memset(void* dest, int value, unsigned long size);
struct Quat {
    float x, y, z, w;
};
extern "C" void quatNormalize__FP4Quat(Quat* q);
extern "C" void matUntransform__FP6Point3P8Matrix33T0(float* out, float* matrix, float* in);

extern "C" void norrath_body_init(float* body, float p0, float p1, float p2, float p3, float p4) {
    const float e2 = p4 * p4;
    const float d2 = p3 * p3;
    const float c2 = p2 * p2;
    const float scale = p1 * p0;
    memset(body, 0, 152);
    const float de = d2 + e2;
    const float cd = c2 + d2;
    const float ce = c2 + e2;
    unsigned state = static_cast<unsigned>(eRandState);
    state = state * 0x19660du + 0x3c6ef35fu;
    const float t1 = scale * de;
    const float t2 = scale * cd;
    const float t3 = scale * ce;
    body[9] = 1.0f;
    const float inverse = 1.0f / p0;
    const float fraction = static_cast<float>(static_cast<int>(state >> 16)) * 0.0000152587890625f;
    body[1] = 12.0f / t1;
    body[0] = inverse;
    body[3] = 12.0f / t2;
    body[2] = 12.0f / t3;
    eRandState = static_cast<int>(state);
    body[10] = fraction;
    Quat* q = reinterpret_cast<Quat*>(body + 7);
    quatNormalize__FP4Quat(q);
    body[14] = 0.0f;
    body[15] = 0.0f;
    body[16] = 0.0f;
    quatNormalize__FP4Quat(q);
    float* m = body + 17;
    const float x = q->x, y = q->y, z = q->z, w = q->w;
    const float n = x * x + y * y + z * z + w * w;
    const float s = 2.0f / n;
    const float xs = x * s, ys = y * s, zs = z * s;
    const float xx = x * xs, yy = y * ys, zz = z * zs;
    const float xy = x * ys, xz = x * zs, yz = y * zs;
    const float wx = w * xs, wy = w * ys, wz = w * zs;
    m[5] = yz - wx;
    m[2] = xz + wy;
    m[1] = xy - wz;
    m[8] = 1.0f - (xx + yy);
    m[0] = 1.0f - (yy + zz);
    m[4] = 1.0f - (xx + zz);
    m[3] = xy + wz;
    m[6] = xz - wy;
    m[7] = yz + wx;
    matUntransform__FP6Point3P8Matrix33T0(body + 35, body + 26, body + 14);
}
