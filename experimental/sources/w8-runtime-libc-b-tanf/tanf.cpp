// Observed tanf at 0x002f1550, 120 bytes, in the usual fdlibm shape. With
// the sign bit cleared, an argument whose bits are at most 0x3f490fda (about
// pi/4) calls __kernel_tanf(x, 0, 1). A larger argument calls
// __ieee754_rem_pio2f(x, y) (0x002f5760) first, which fills a two-float
// remainder and returns the quadrant count n, then calls
// __kernel_tanf(y[0], y[1], 1 - ((n & 1) << 1)). The result is returned.
// The original keeps the argument in memory to read its bits; that is not
// reproduced.
extern "C" float __kernel_tanf(float x, float y, int selector);
extern "C" int __ieee754_rem_pio2f(float x, float* remainder);

extern "C" float tanf(float x) {
    union { float value; unsigned bits; } word;
    word.value = x;
    float remainder[2];
    if ((word.bits & 0x7fffffffu) <= 0x3f490fdau) return __kernel_tanf(x, 0.0f, 1);
    const int quadrant = __ieee754_rem_pio2f(x, remainder);
    return __kernel_tanf(remainder[0], remainder[1], 1 - ((quadrant & 1) << 1));
}
