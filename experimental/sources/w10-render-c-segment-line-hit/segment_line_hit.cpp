// Observed segmentLineHit__FP6Point3N50 at 0x00157430, 564 bytes (compiled size may
// differ). Six Point3 pointers (the mangling repeats the first type for the last
// five) and an int result of 0 or 1. Called p0..p3, e and out in order:
//   c = p2 (plane normal-like vector), w = p3 - e, u = p0 - e.
//   den = (w.x*c.x + w.y*c.y) + w.z*c.z; if 0 <= den the result is 0.
//   num = (u.x*c.x + u.y*c.y) + u.z*c.z.
//   The denominator is then clamped away from zero (a positive value below 0.01f is
//   raised to 0.01f, a negative value above -0.01f lowered to -0.01f), which can
//   only take effect for the negative case since den < 0 here.
//   s = num / den; if s < 0 or 1.0f < s the result is 0.
//   *out = e + w*s (written before the final test).
//   g = p0 - p1, h = *out - p1, len = |g|, g' = g * (1.0f/len),
//   proj = (g'.x*h.x + g'.y*h.y) + g'.z*h.z.
//   The result is 0 when proj < -0.25f or len + 0.25f < proj, otherwise 1.
// The literals 0.01f and -0.01f are 4-byte pool floats the original reads through
// its global pointer; their values were read from the image.
// Inferred: a segment p3..e (or its extension) tested against a plane through p0
// with normal p2, then checked to lie within a span from p1 along p0. Unknown: the
// real meaning of each point.
struct Point3 {
    float x, y, z;
};

extern "C" int segmentLineHit__FP6Point3N50(const Point3* p0, const Point3* p1, const Point3* p2, const Point3* p3,
                                            const Point3* e, Point3* out) {
    const float wx = p3->x - e->x, wy = p3->y - e->y, wz = p3->z - e->z;
    const float den0 = (wx * p2->x + wy * p2->y) + wz * p2->z;
    if (0.0f <= den0) return 0;
    const float ux = p0->x - e->x, uy = p0->y - e->y, uz = p0->z - e->z;
    float den = (wx * p2->x + wy * p2->y) + wz * p2->z;
    const float num = (ux * p2->x + uy * p2->y) + uz * p2->z;
    if (0.0f < den) {
        if (den < 0.01f) den = 0.01f;
    }
    if (den < 0.0f) {
        if (-0.01f < den) den = -0.01f;
    }
    const float s = num / den;
    if (s < 0.0f) return 0;
    if (1.0f < s) return 0;
    out->x = e->x + wx * s;
    out->y = e->y + wy * s;
    out->z = e->z + wz * s;
    const float hz = out->z - p1->z, hx = out->x - p1->x, hy = out->y - p1->y;
    const float gz = p0->z - p1->z, gx = p0->x - p1->x, gy = p0->y - p1->y;
    const float len = __builtin_sqrtf((gx * gx + gy * gy) + gz * gz);
    const float inv = 1.0f / len;
    const float proj = (gx * inv * hx + gy * inv * hy) + gz * inv * hz;
    if (proj < -0.25f) return 0;
    if (len + 0.25f < proj) return 0;
    return 1;
}
