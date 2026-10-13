// Observed scalePatchLimit__7VehicleiP6Point3 at 0x00276260, 272 bytes.
// Arguments: the vehicle, an index i, and a Point3 passed by value (here the
// address of a caller-owned copy; the original normalises its own copy, so the
// caller's value is not changed). Returns a float:
//   n = direction scaled to length one (each component times rsqrt.s of the
//       sum of squares, summed x, y, z in that order);
//   d = R_i . n with R_i the Point3 at vehicle + 1644 + 12 * i, summed in the
//       order x, y, z in single precision;
//   a = |d|, taken through the original's soft double routines fptodp, dpcmp,
//       dpsub (0 - d when d < 0) and dptofp;
//   result = f + (1.0 - f) * a, with f the float at vehicle + 764.
// doDriveline__7Vehicle passes the result on as a force limit; calling it a
// limit is taken from the name only. Nothing here was executed on the EE.
struct __attribute__((may_alias)) Point3 { float x, y, z; };

typedef unsigned long long Double;

extern "C" {
Double fptodp(float value);
int dpcmp(Double a, Double b);
Double dpsub(Double a, Double b);
float dptofp(Double a);
}

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

extern "C" float scalePatchLimit__7VehicleiP6Point3(unsigned char* vehicle, int index, const Point3* direction) {
    Point3 n = *direction;
    const Point3* const r = reinterpret_cast<const Point3*>(vehicle + 1644 + 12 * index);
    const float f = *reinterpret_cast<AliasedFloat*>(vehicle + 764);
    const float rest = 1.0f - f;
    const float inverse = rsqrt_s(1.0f, n.x * n.x + n.y * n.y + n.z * n.z);
    n.x = n.x * inverse;
    n.y = n.y * inverse;
    n.z = n.z * inverse;
    const float dot = r->x * n.x + r->y * n.y + r->z * n.z;
    Double d = fptodp(dot);
    if (dpcmp(d, 0) < 0) d = dpsub(0, d);
    return f + rest * dptofp(d);
}
