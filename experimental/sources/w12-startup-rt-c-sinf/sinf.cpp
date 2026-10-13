// Observed sinf at 0x002f1468, 232 bytes, in the usual fdlibm shape. With the sign bit
// cleared, an argument whose bits are at most 0x3f490fda (about pi/4) returns
// __kernel_sinf(x, 0, 0). A larger argument calls __ieee754_rem_pio2f(x, y) (0x002f5760),
// which fills a two-float remainder and returns the quadrant count n, then n & 3 picks
// the result: 0 is __kernel_sinf(y0, y1, 1), 1 is __kernel_cosf(y0, y1), 2 is the negated
// __kernel_sinf(y0, y1, 1), 3 is the negated __kernel_cosf(y0, y1). The kernels are
// __kernel_sinf at 0x002f75f8 and __kernel_cosf at 0x002f6c40; the last argument of the
// sine kernel is passed in a0 and the second float in the second float argument register.
// The original keeps the argument in memory to read its bits; that is not reproduced.
extern "C" float __kernel_sinf(float x, float y, int selector);
extern "C" float __kernel_cosf(float x, float y);
extern "C" int __ieee754_rem_pio2f(float x, float* remainder);

extern "C" float sinf(float x) {
    union { float value; unsigned bits; } word;
    word.value = x;
    float remainder[2];
    if ((word.bits & 0x7fffffffu) <= 0x3f490fdau) return __kernel_sinf(x, 0.0f, 0);
    const int quadrant = __ieee754_rem_pio2f(x, remainder);
    switch (quadrant & 3) {
    case 0: return __kernel_sinf(remainder[0], remainder[1], 1);
    case 1: return __kernel_cosf(remainder[0], remainder[1]);
    case 2: return -__kernel_sinf(remainder[0], remainder[1], 1);
    default: return -__kernel_cosf(remainder[0], remainder[1]);
    }
}
