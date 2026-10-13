// Observed rayHit__FP6Point3N30 at 0x00157308, 292 bytes (compiled size may differ).
// Four Point3 pointers p0..p3 (the mangling repeats the first type for the last
// three) and a float result. Reading, with a = p1 and n = p3:
//   d = (a.x*n.x + a.y*n.y) + a.z*n.z
//   t = (n.x*(p0.x-p2.x) + n.y*(p0.y-p2.y)) + n.z*(p0.z-p2.z)
// If 0 <= d or t < 0 the result is -1.0f (a NaN in d or t does not take that exit).
// Otherwise the vector n*t is dotted with a and negated, giving e; when
// 0.001f < |e| the result is t * (t / e), else t * (t * 0x1.f40002p+9f).
// The two literals are 4-byte pool floats the original reads relative to its global
// pointer. Their bit patterns, read from the image, are 0x3a83126e and 0x447a0001,
// one unit in the last place away from the nearest floats to 0.001 and 1000.0
// (0x3a83126f and 0x447a0000), so they are written as exact hexadecimal floats.
// Inferred: a ray-versus-plane style distance test. Unknown: the roles of the four
// points and what the returned number measures.
struct Point3 {
    float x, y, z;
};

extern "C" float rayHit__FP6Point3N30(const Point3* p0, const Point3* p1, const Point3* p2, const Point3* p3) {
    const float d = (p1->x * p3->x + p1->y * p3->y) + p1->z * p3->z;
    const float t = (p3->x * (p0->x - p2->x) + p3->y * (p0->y - p2->y)) + p3->z * (p0->z - p2->z);
    if (0.0f <= d || t < 0.0f) return -1.0f;
    const float px = p3->x * t, py = p3->y * t, pz = p3->z * t;
    const float e = -((px * p1->x + py * p1->y) + pz * p1->z);
    const float mag = e < 0.0f ? -e : e;       // abs.s
    if (0x1.0624dcp-10f < mag) return t * (t / e);
    return t * (t * 0x1.f40002p+9f);
}
