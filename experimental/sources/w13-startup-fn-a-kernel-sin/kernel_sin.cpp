// Observed __kernel_sin at 0x002f6a08, 568 bytes: the double-precision sine kernel, __kernel_sin(x, y, iy), with
// every double carried as one 64-bit value (the image's soft-double helpers take and return such values).
// x is the reduced argument (|x| about pi/4 or less), y its low-order correction and iy a flag: zero means y is
// ignored. In order, as read from the code:
//  1. ix = high word of x without the sign. When ix <= 0x3e3fffff and dptoli(x) is zero, return x.
//  2. z = x*x, v = z*x, and r = k5 + z*(k4 + z*(k3 + z*(k2 + z*k1))) with the five constants below.
//  3. iy == 0: return x + v*(s1 + z*r).
//     otherwise: return x - ((z*(0.5*y - v*r) - y) - v*s1).
// Every operation is one call of dpmul, dpadd or dpsub, in the order the code makes them. The constants are the
// eight-byte table words in the image (hex below); the last two are the same number, which the code loads from
// two places. Inferred: this is the usual fdlibm sine kernel in soft double and s1 is its -1/6 term; names are mine.
// Not authored here: dpmul, dpadd, dpsub, dptoli.
extern "C" {
typedef unsigned long long Dp;
Dp dpmul(Dp left, Dp right);
Dp dpadd(Dp left, Dp right);
Dp dpsub(Dp left, Dp right);
int dptoli(Dp value);

Dp __kernel_sin(Dp x, Dp y, int iy) {
    const int ix = static_cast<int>(x >> 32) & 0x7fffffff;
    if (ix <= 0x3e3fffff) {
        if (dptoli(x) == 0) return x;
    }
    const Dp z = dpmul(x, x);
    const Dp v = dpmul(z, x);
    Dp r = dpmul(z, 0x3de5d93a5acfd57bull);
    r = dpadd(r, 0xbe5ae5e68a2b9ceaull);
    r = dpmul(z, r);
    r = dpadd(r, 0x3ec71de357b1fe7cull);
    r = dpmul(z, r);
    r = dpadd(r, 0xbf2a01a019c161d5ull);
    r = dpmul(z, r);
    r = dpadd(r, 0x3f8111111110f8a6ull);
    const Dp s1 = 0xbfc5555555555549ull;
    if (iy == 0) {
        Dp t = dpmul(z, r);
        t = dpadd(t, s1);
        t = dpmul(v, t);
        return dpadd(x, t);
    }
    const Dp half_y = dpmul(y, 0x3fe0000000000000ull);
    const Dp vr = dpmul(v, r);
    Dp t = dpsub(half_y, vr);
    t = dpmul(z, t);
    t = dpsub(t, y);
    t = dpsub(t, dpmul(v, s1));
    return dpsub(x, t);
}
}
