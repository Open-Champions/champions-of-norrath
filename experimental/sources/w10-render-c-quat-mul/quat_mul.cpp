// Observed quatMul__FP4QuatN20 at 0x00159ec0, 208 bytes (compiled size may differ).
// Three pointers: destination, left operand, right operand, each four floats at
// offsets 0, 4, 8 and 12. The result is built in a 16-byte stack temporary and then
// copied over the destination, so the destination may alias either operand.
// With the left operand as (ax, ay, az, aw) and the right as (bx, by, bz, bw) the
// four results are
//   x = (aw*bx + bw*ax) + (ay*bz - az*by)
//   y = (aw*by + bw*ay) + (az*bx - ax*bz)
//   z = (aw*bz + bw*az) + (ax*by - ay*bx)
//   w = aw*bw - (ax*bx + ay*by + az*bz)
// which is the Hamilton product with w last. Nothing is returned.
// Inferred: component order x, y, z, w (matches the other Quat routines).
// Unknown: whether the original callers rely on a particular operand order beyond
// the argument order observed here.
struct Quat {
    float x, y, z, w;
};

extern "C" void quatMul__FP4QuatN20(Quat* out, const Quat* a, const Quat* b) {
    const float ax = a->x, ay = a->y, az = a->z, aw = a->w;
    const float bx = b->x, by = b->y, bz = b->z, bw = b->w;
    const float dot = ax * bx + ay * by + az * bz;
    Quat r;
    r.x = (aw * bx + bw * ax) + (ay * bz - az * by);
    r.y = (aw * by + bw * ay) + (az * bx - ax * bz);
    r.z = (aw * bz + bw * az) + (ax * by - ay * bx);
    r.w = aw * bw - dot;
    *out = r;
}
