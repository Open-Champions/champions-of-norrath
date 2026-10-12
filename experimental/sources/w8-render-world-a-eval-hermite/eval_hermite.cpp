// Observed evalHermite__Ffffff at 0x00159330, 96 bytes, five single-float
// arguments in f12..f16 and a single-float result. With t = f12:
//   t2 = t * t, t3 = t2 * t
//   result = (((2*t3 - 3*t2) * f13 + f13) + (3*t2 - 2*t3) * f14
//             + (t3 - 2*t2 + t) * f15) + (t3 - t2) * f16
// The operation order is the order of the image; every step is a separate
// single-precision multiply, add or subtract.
// Inferred: this is the cubic Hermite blend. f13 is the value at t = 0, f14 the
// value at t = 1, and f15 and f16 the tangents at t = 0 and t = 1. The parameter
// names below follow that reading; the image shows only the arithmetic.
// Unknown: the callers' units for t and the tangents.
extern "C" float evalHermite__Ffffff(float t, float start, float end, float start_tangent, float end_tangent) {
    const float t2 = t * t;
    const float t3 = t2 * t;
    const float h00 = 2.0f * t3 - 3.0f * t2;
    const float h01 = 3.0f * t2 - 2.0f * t3;
    const float h10 = t3 - 2.0f * t2 + t;
    const float h11 = t3 - t2;
    return ((h00 * start + start) + h01 * end + h10 * start_tangent) + h11 * end_tangent;
}
