// Observed cosf at 0x002f1290, 212 bytes, pinned SLUS_205.65. Single-float cosine in the usual
// reduce-then-evaluate form. The argument arrives in $f12; its raw bits are read with the sign
// cleared (signed comparison). Inferred from the code only:
//   - |x| bits <= 0x3f490fd8 (about pi/4): __kernel_cosf(x, 0) with no reduction.
//   - otherwise __ieee754_rem_pio2f(x, y) fills two floats y[0], y[1] and returns the quadrant n;
//     by n & 3: 0 -> __kernel_cosf(y0, y1); 1 -> -__kernel_sinf(y0, y1, 1);
//     2 -> -__kernel_cosf(y0, y1); 3 -> __kernel_sinf(y0, y1, 1).
// There is no separate branch for infinities or NaNs; they follow the reduction path. The
// negation is applied to the helper's result.
// Unknown: the exact special-value behaviour, which belongs to the three helpers (not authored here).
extern "C" {
float __kernel_cosf(float x, float y);
float __kernel_sinf(float x, float y, int iy);
int __ieee754_rem_pio2f(float x, float* y);

float cosf(float x) {
    float y[2];
    int bits;
    __builtin_memcpy(&bits, &x, 4);
    const int magnitude = bits & 0x7fffffff;
    if (magnitude <= 0x3f490fd8) return __kernel_cosf(x, 0.0f);
    const int n = __ieee754_rem_pio2f(x, y);
    switch (n & 3) {
        case 0: return __kernel_cosf(y[0], y[1]);
        case 1: return -__kernel_sinf(y[0], y[1], 1);
        case 2: return -__kernel_cosf(y[0], y[1]);
        default: return __kernel_sinf(y[0], y[1], 1);
    }
}
}
