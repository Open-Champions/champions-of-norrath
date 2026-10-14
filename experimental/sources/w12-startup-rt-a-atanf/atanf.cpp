// Observed atanf at 0x002f0f28, 652 bytes, pinned SLUS_205.65. Single-float arc tangent, argument in
// $f12. Structure read from the code (the usual argument-halving scheme with a short odd/even
// polynomial); all comparisons are on the raw bits with the sign cleared (ix) and signed:
//   - ix >= 0x50800000 (about 2^34, also NaN and infinity): +/- (atanhi[3] + atanlo[3]), sign by the
//     sign of x, with no separate NaN test.
//   - ix < 0x3ee00000 (|x| < 0.4375): id = -1; when also ix < 0x31000000 and x + 1e30f > 1, x is
//     returned unchanged.
//   - otherwise x = |x| (fabsf) and it is reduced by interval:
//       ix < 0x3f300000: id 0, x = (2x - 1) / (2 + x)
//       ix < 0x3f980000: id 1, x = (x - 1) / (x + 1)
//       ix < 0x401c0000: id 2, x = (x - 1.5) / (1 + 1.5x)
//       else:            id 3, x = -1 / x
//   - z = x*x, w = z*z; s1 = z * (a0 + w*(a2 + w*(a4 + w*(a6 + w*(a8 + w*a10))))) and
//     s2 = w * (a1 + w*(a3 + w*(a5 + w*(a7 + w*a9)))), with a = the 11 floats of the table at 0x0047cb10.
//     id -1: x - x*(s1 + s2). Otherwise hi[id] - ((x*(s1 + s2) - lo[id]) - x), negated when the
//     original argument was negative (hi, lo = the 4-float tables at 0x0047caf0 and 0x0047cb00).
// The three tables are imported under the names of the existing byte views of math-atan-tables.
// Unknown: the exact special-value behaviour on the EE FPU (no EE execution was done).
extern "C" {
float fabsf(float x);

typedef float __attribute__((may_alias)) AliasedFloat;
extern unsigned char norrath_animation_math_atan_high_view[16] __attribute__((aligned(4)));
extern unsigned char norrath_animation_math_atan_low_view[16] __attribute__((aligned(4)));
extern unsigned char norrath_animation_math_atan_coefficients_view[44] __attribute__((aligned(4)));

float atanf(float x) {
    const AliasedFloat* const hi = reinterpret_cast<const AliasedFloat*>(norrath_animation_math_atan_high_view);
    const AliasedFloat* const lo = reinterpret_cast<const AliasedFloat*>(norrath_animation_math_atan_low_view);
    const AliasedFloat* const a = reinterpret_cast<const AliasedFloat*>(norrath_animation_math_atan_coefficients_view);
    int hx;
    __builtin_memcpy(&hx, &x, 4);
    const int ix = hx & 0x7fffffff;
    int id;
    if (ix >= 0x50800000) {
        if (hx > 0) return hi[3] + lo[3];
        return -hi[3] - lo[3];
    }
    if (ix < 0x3ee00000) {
        if (ix < 0x31000000) {
            if (1.0e30f + x > 1.0f) return x;
        }
        id = -1;
    } else {
        x = fabsf(x);
        if (ix < 0x3f980000) {
            if (ix < 0x3f300000) {
                id = 0;
                x = (2.0f * x - 1.0f) / (2.0f + x);
            } else {
                id = 1;
                x = (x - 1.0f) / (x + 1.0f);
            }
        } else {
            if (ix < 0x401c0000) {
                id = 2;
                x = (x - 1.5f) / (1.0f + 1.5f * x);
            } else {
                id = 3;
                x = -1.0f / x;
            }
        }
    }
    const float z = x * x;
    const float w = z * z;
    const float s1 = z * (a[0] + w * (a[2] + w * (a[4] + w * (a[6] + w * (a[8] + w * a[10])))));
    const float s2 = w * (a[1] + w * (a[3] + w * (a[5] + w * (a[7] + w * a[9]))));
    if (id < 0) return x - x * (s1 + s2);
    const float result = hi[id] - ((x * (s1 + s2) - lo[id]) - x);
    return hx < 0 ? -result : result;
}
}
