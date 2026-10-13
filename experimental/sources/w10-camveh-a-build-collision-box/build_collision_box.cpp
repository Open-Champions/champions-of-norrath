// Observed buildCollisionBox__7Vehicle at 0x00276370, 540 bytes. Nothing is
// returned. Let M be the nine floats at vehicle+1116 (rows k = 0..2 at
// +1116 + 12 * k) and pos the Point3 at +16.
//   1. For each row k the first two elements (x, y) are scaled to length one
//      with rsqrt.s of x*x + y*y and kept as B_k = (x', y').
//   2. For j = 0..3, with P_j the Point3 at +404 + 12 * j:
//      Q_j.x = (B_0.x * P_j.x + B_1.x * P_j.y) + B_2.x * P_j.z;
//      Q_j.y = (B_0.y * P_j.x + B_1.y * P_j.y) + B_2.y * P_j.z;
//      the Point3 at +452 + 12 * j becomes (Q_j.x + pos.x, Q_j.y + pos.y, 0 + pos.z);
//      then matTransform__FP6Point3P8Matrix33T0(out = +548 + 12 * j, M,
//      in = +500 + 12 * j) is called, the z of the output (+556 + 12 * j) is
//      set to 0.0, and the output is scaled to length one with rsqrt.s of
//      x*x + y*y + z*z (the scale applied z, x, y in that order).
// Only the +404 table is read besides M and pos; the +500 table is an input
// written elsewhere. The first table of four points looks like body-box
// corners and the +548 results like unit edge directions in the xy plane, but
// that is an inference; the original fills no other fields. Nothing here was
// executed on the EE.
struct __attribute__((may_alias)) Point3 { float x, y, z; };

extern "C" void matTransform__FP6Point3P8Matrix33T0(Point3* out, const void* matrix, const Point3* in);

typedef float AliasedFloat __attribute__((may_alias));

static inline float rsqrt_s(float numerator, float value) {
#if defined(__mips__)
    float out;
    __asm__("rsqrt.s %0, %1, %2" : "=f"(out) : "f"(numerator), "f"(value));
    return out;
#else
    return numerator / __builtin_sqrtf(value);
#endif
}

extern "C" void buildCollisionBox__7Vehicle(unsigned char* vehicle) {
    Point3 rows[3];
    Point3 basis[3];
    const AliasedFloat* const m = reinterpret_cast<AliasedFloat*>(vehicle + 1116);
    for (int k = 0; k < 3; ++k) {
        const float x = m[3 * k];
        const float y = m[3 * k + 1];
        const float inverse = rsqrt_s(1.0f, x * x + y * y);
        rows[k].z = 0.0f;
        rows[k].x = x * inverse;
        rows[k].y = y * inverse;
        basis[k] = rows[k];
    }
    const Point3* const position = reinterpret_cast<const Point3*>(vehicle + 16);
    for (int j = 0; j < 4; ++j) {
        Point3* const p = reinterpret_cast<Point3*>(vehicle + 404 + 12 * j);
        Point3* const q = reinterpret_cast<Point3*>(vehicle + 452 + 12 * j);
        Point3* const out = reinterpret_cast<Point3*>(vehicle + 548 + 12 * j);
        q->x = (basis[0].x * p->x + basis[1].x * p->y) + basis[2].x * p->z;
        q->y = (basis[0].y * p->x + basis[1].y * p->y) + basis[2].y * p->z;
        q->z = 0.0f;
        q->x = q->x + position->x;
        q->y = q->y + position->y;
        q->z = q->z + position->z;
        matTransform__FP6Point3P8Matrix33T0(out, vehicle + 1116, reinterpret_cast<const Point3*>(vehicle + 500 + 12 * j));
        out->z = 0.0f;
        const float inverse = rsqrt_s(1.0f, out->x * out->x + out->y * out->y + out->z * out->z);
        out->z = out->z * inverse;
        out->x = out->x * inverse;
        out->y = out->y * inverse;
    }
}
