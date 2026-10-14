// Observed quatToMat__FP8Matrix33P4Quat at 0x0015a2b0, 208 bytes (compiled size may differ).
// Reads four floats (offsets 0, 4, 8, 12) from the quaternion and writes nine floats
// (offsets 0..32 in steps of 4) of the output. The squared length n is summed in the
// order offset 12, 0, 4, 8; s = 2 / n (a real divide, no zero check). With x, y, z, w =
// offsets 0, 4, 8, 12, the output words by byte offset are:
//    0: 1 - (yy + zz)     4: xy + wz     8: xz - wy
//   12: xy - wz          16: 1 - (xx + zz)   20: yz + wx
//   24: xz + wy          28: yz - wx    32: 1 - (xx + yy)
// where xx is x*(x*s), xy is x*(y*s), wz is w*(z*s) and so on. Nothing is returned.
// Inferred: Matrix33 is nine contiguous floats. Unknown: whether the result is a
// rotation for a non-unit quaternion beyond the 2/n scale shown here.
struct Quat {
    float x, y, z, w;
};

extern "C" void quatToMat__FP8Matrix33P4Quat(float* m, const Quat* q) {
    const float x = q->x, y = q->y, z = q->z, w = q->w;
    const float n = x * x + y * y + z * z + w * w;
    const float s = 2.0f / n;
    const float xs = x * s, ys = y * s, zs = z * s;
    const float xx = x * xs, yy = y * ys, zz = z * zs;
    const float xy = x * ys, xz = x * zs, yz = y * zs;
    const float wx = w * xs, wy = w * ys, wz = w * zs;
    m[7] = yz - wx;
    m[6] = xz + wy;
    m[3] = xy - wz;
    m[8] = 1.0f - (xx + yy);
    m[0] = 1.0f - (yy + zz);
    m[4] = 1.0f - (xx + zz);
    m[1] = xy + wz;
    m[2] = xz - wy;
    m[5] = yz + wx;
}
