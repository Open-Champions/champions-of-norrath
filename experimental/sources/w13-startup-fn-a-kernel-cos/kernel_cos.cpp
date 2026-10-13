// Observed __kernel_cos at 0x002f5be0, 724 bytes: the double-precision cosine kernel, __kernel_cos(x, y), with
// every double carried as one 64-bit value (the image's soft-double helpers take and return such values).
// x is the reduced argument (|x| about pi/4 or less) and y its low-order correction. In order, as read from the code:
//  1. ix = high word of x without the sign. When ix <= 0x3e3fffff and dptoli(x) is zero, return 1.0.
//  2. z = x*x and r = z*(c6 + z*(c5 + z*(c4 + z*(c3 + z*(c2 + z*c1))))), with the six constants below.
//  3. ix < 0x3fd33333: return 1 - (0.5*z - (z*r - x*y)).
//     Otherwise qx is 0.28125 when ix > 0x3fe90000 and else the double whose high word is ix - 0x00200000 and
//     whose low word is zero; hz = 0.5*z - qx, a = 1 - qx, and the result is a - (hz - (z*r - x*y)).
// Every operation is one call of dpmul, dpadd or dpsub, in the order the code makes them. The constants are
// the eight-byte table words in the image. Inferred: this is the usual fdlibm cosine kernel in soft double;
// names are mine. The image builds qx through a register that held an earlier value, but the low word is cleared
// afterwards, so the earlier value cannot reach the result.
// Not authored here: dpmul, dpadd, dpsub, dptoli.
extern "C" {
typedef unsigned long long Dp;
Dp dpmul(Dp left, Dp right);
Dp dpadd(Dp left, Dp right);
Dp dpsub(Dp left, Dp right);
int dptoli(Dp value);

Dp __kernel_cos(Dp x, Dp y) {
    const Dp one = 0x3ff0000000000000ull;
    const Dp half = 0x3fe0000000000000ull;
    const int ix = static_cast<int>(x >> 32) & 0x7fffffff;
    if (ix <= 0x3e3fffff) {
        if (dptoli(x) == 0) return one;
    }
    const Dp z = dpmul(x, x);
    Dp p = dpmul(z, 0xbda8fae9be8838d4ull);
    p = dpadd(p, 0x3e21ee9ebdb4b1c4ull);
    p = dpmul(z, p);
    p = dpadd(p, 0xbe927e4f809c52adull);
    p = dpmul(z, p);
    p = dpadd(p, 0x3efa01a019cb158full);
    p = dpmul(z, p);
    p = dpadd(p, 0xbf56c16c16c15176ull);
    p = dpmul(z, p);
    p = dpadd(p, 0x3fa555555555554cull);
    const Dp r = dpmul(z, p);
    if (ix < 0x3fd33333) {
        const Dp hz = dpmul(z, half);
        const Dp zr = dpmul(z, r);
        const Dp xy = dpmul(x, y);
        Dp t = dpsub(zr, xy);
        t = dpsub(hz, t);
        return dpsub(one, t);
    }
    Dp qx = 0x3fd2000000000000ull;
    if (ix <= 0x3fe90000) qx = static_cast<Dp>(static_cast<unsigned>(ix - 0x00200000)) << 32;
    Dp hz = dpmul(z, half);
    hz = dpsub(hz, qx);
    const Dp a = dpsub(one, qx);
    const Dp zr = dpmul(z, r);
    const Dp xy = dpmul(x, y);
    Dp t = dpsub(zr, xy);
    t = dpsub(hz, t);
    return dpsub(a, t);
}
}
