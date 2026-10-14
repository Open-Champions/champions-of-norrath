// Observed quatNormalize__FP4Quat at 0x00159f90, 100 bytes (compiled size may differ).
// Four floats at offsets 0, 4, 8 and 12. The squared length is summed in the order
// offset 12, offset 0, offset 4, offset 8, then one hardware reciprocal square root
// (rsqrt.s of 1.0 by that sum) scales every component in place. There is no zero
// check, so a zero quaternion produces infinities or NaNs. Nothing is returned.
// Inferred: the layout is x, y, z, w with w last. Unknown: the Quat type name's field names.
struct Quat {
    float x, y, z, w;
};

extern "C" void quatNormalize__FP4Quat(Quat* q) {
    const float sum = q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z;
    const float scale = 1.0f / __builtin_sqrtf(sum);
    q->z = q->z * scale;
    q->w = q->w * scale;
    q->x = q->x * scale;
    q->y = q->y * scale;
}
