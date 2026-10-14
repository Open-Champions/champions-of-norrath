// Observed quatToMat__FP8Matrix34P4Quat at 0x0015a380, 220 bytes (compiled size may differ).
// Identical arithmetic to quatToMat__FP8Matrix33P4Quat at 0x0015a2b0 for the first nine
// words (see that file's header for the formulas). It also stores zero at byte offsets
// 36, 40 and 44, the last three words of the twelve-word Matrix34. Nothing at or past
// offset 48 is touched. Nothing is returned.
// Unknown: the exact Matrix34 member names and whether the clear is intentional.
struct Quat {
    float x, y, z, w;
};

extern "C" void quatToMat__FP8Matrix34P4Quat(float* m, const Quat* q) {
    const float x = q->x, y = q->y, z = q->z, w = q->w;
    const float n = x * x + y * y + z * z + w * w;
    ((unsigned*)m)[11] = 0u;
    ((unsigned*)m)[9] = 0u;
    ((unsigned*)m)[10] = 0u;
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
