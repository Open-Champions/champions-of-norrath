/* Observed sin at 0x002f0df0, 308 bytes, pinned SLUS_205.65. Double-precision sine in
 * the fdlibm shape, with every double carried as one 64-bit value (the soft-double
 * helpers take and return such values). With hx the high word of x and its sign bit
 * cleared:
 *   hx <= 0x3fe921fb (about pi/4): __kernel_sin(x, 0, 0), at 0x002f6a08.
 *   hx > 0x7fefffff (infinity or NaN): dpsub(x, x), at 0x002fb618.
 *   otherwise __ieee754_rem_pio2(x, y) (0x002f3c78) fills two doubles and returns the
 *   quadrant count n; n & 3 picks the result: 0 is __kernel_sin(y0, y1, 1), 1 is
 *   __kernel_cos(y0, y1) (0x002f5be0), 2 is dpsub(0, __kernel_sin(y0, y1, 1)), 3 is
 *   dpsub(0, __kernel_cos(y0, y1)).
 * The negation is the subtraction from zero, as the code does it.
 * Unknown: the bodies of the callees. Nothing was executed on the EE.
 */
extern "C" {
typedef unsigned long long Dp;
Dp __kernel_sin(Dp x, Dp y, int selector);
Dp __kernel_cos(Dp x, Dp y);
int __ieee754_rem_pio2(Dp x, Dp* remainder);
Dp dpsub(Dp left, Dp right);

Dp sin(Dp x) {
    Dp remainder[2];
    const int high = static_cast<int>(x >> 32) & 0x7fffffff;
    if (high <= 0x3fe921fb) return __kernel_sin(x, 0, 0);
    if (high > 0x7fefffff) return dpsub(x, x);
    const int quadrant = __ieee754_rem_pio2(x, remainder);
    switch (quadrant & 3) {
    case 0: return __kernel_sin(remainder[0], remainder[1], 1);
    case 1: return __kernel_cos(remainder[0], remainder[1]);
    case 2: return dpsub(0, __kernel_sin(remainder[0], remainder[1], 1));
    default: return dpsub(0, __kernel_cos(remainder[0], remainder[1]));
    }
}
}
