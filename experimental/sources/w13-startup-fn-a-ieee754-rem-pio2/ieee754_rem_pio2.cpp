// Observed __ieee754_rem_pio2 at 0x002f3c78, 1464 bytes: argument reduction for the double-precision sine and
// cosine, __ieee754_rem_pio2(x, y) -> n, with every double carried as one 64-bit value (the image's soft-double
// helpers take and return such values) and y a pointer to two such values. It stores a two-part remainder of x
// modulo pi/2 in y[0] (head) and y[1] (tail) and returns the quadrant count n (negative for negative x).
// With hx the high word of x and ix = hx without the sign, read from the code in this order:
//  1. ix <= 0x3fe921fb: y = {x, 0}, return 0.
//  2. ix < 0x4002d97c (below 3*pi/4): subtract (hx > 0) or add (hx <= 0) pio2_1 from x, then a two-step tail
//     with pio2_1t, or, when ix == 0x3ff921fb, the next pair pio2_2 and pio2_2t. Return 1 or -1.
//  3. ix <= 0x413921fb (below about 2^20 * pi/2): n = (int)(|x| * invpio2 + 0.5), fn = (double)n,
//     r = |x| - fn*pio2_1, w = fn*pio2_1t. When n < 32 and ix differs from the table word npio2_hw[n-1], y[0] =
//     r - w. Otherwise the exponent of y[0] = r - w is compared with that of x: more than 16 bits lost
//     repeats the step with pio2_2 / pio2_2t, and more than 49 bits lost once more with pio2_3 / pio2_3t.
//     Then y[1] = (r - y[0]) - w, and for negative x both are negated (subtracted from zero) and -n is returned.
//  4. ix >= 0x7ff00000 (infinity or NaN): y[0] = y[1] = x - x, return 0.
//  5. otherwise: e0 = (ix >> 20) - 1046, z is x with its exponent reduced by e0, the first two 24-bit pieces
//     of z go to tx[0..1] (each is (double)(int)z, then z = (z - piece) * 2^24), tx[2] = z, nx is 3 reduced
//     past trailing zero pieces (compared with dpcmp), and n = __kernel_rem_pio2(tx, y, e0, nx, 2, two_over_pi)
//     with the table below; for negative x both results are negated and -n is returned.
// The constants and the two tables are the eight-byte and four-byte words found in the image (the code loads
// separate copies of pio2_1, pio2_1t, pio2_2 and pio2_2t in the positive, negative and medium paths; they are
// equal). Inferred: this is the usual fdlibm routine in soft double and the names are mine. Every operation is
// one soft-double helper call in the order the code makes them.
// Not authored here: dpmul, dpadd, dpsub, dpcmp, dptoli, litodp, fabs, __kernel_rem_pio2.
extern "C" {
typedef unsigned long long Dp;
Dp dpmul(Dp left, Dp right);
Dp dpadd(Dp left, Dp right);
Dp dpsub(Dp left, Dp right);
int dpcmp(Dp left, Dp right);
int dptoli(Dp value);
Dp litodp(int value);
Dp fabs(Dp value);
int __kernel_rem_pio2(Dp* x, Dp* y, int e0, int nx, int prec, const int* two_over_pi);

namespace {
const int npio2_hw[32] = {
    0x3ff921fb, 0x400921fb, 0x4012d97c, 0x401921fb, 0x401f6a7a, 0x4022d97c, 0x4025fdbb, 0x402921fb,
    0x402c463a, 0x402f6a7a, 0x4031475c, 0x4032d97c, 0x40346b9c, 0x4035fdbb, 0x40378fdb, 0x403921fb,
    0x403ab41b, 0x403c463a, 0x403dd85a, 0x403f6a7a, 0x40407e4c, 0x4041475c, 0x4042106c, 0x4042d97c,
    0x4043a28c, 0x40446b9c, 0x404534ac, 0x4045fdbb, 0x4046c6cb, 0x40478fdb, 0x404858eb, 0x404921fb,
};

const int two_over_pi[66] = {
    0xa2f983, 0x6e4e44, 0x1529fc, 0x2757d1, 0xf534dd, 0xc0db62, 0x95993c, 0x439041, 0xfe5163, 0xabdebb, 0xc561b7,
    0x246e3a, 0x424dd2, 0xe00649, 0x2eea09, 0xd1921c, 0xfe1deb, 0x1cb129, 0xa73ee8, 0x8235f5, 0x2ebb44, 0x84e99c,
    0x7026b4, 0x5f7e41, 0x3991d6, 0x398353, 0x39f49c, 0x845f8b, 0xbdf928, 0x3b1ff8, 0x97ffde, 0x05980f, 0xef2f11,
    0x8b5a0a, 0x6d1f6d, 0x367ecf, 0x27cb09, 0xb74f46, 0x3f669e, 0x5fea2d, 0x7527ba, 0xc7ebe5, 0xf17b3d, 0x0739f7,
    0x8a5292, 0xea6bfb, 0x5fb11f, 0x8d5d08, 0x560330, 0x46fc7b, 0x6babf0, 0xcfbc20, 0x9af436, 0x1da9e3, 0x91615e,
    0xe61b08, 0x659985, 0x5f14a0, 0x68408d, 0xffd880, 0x4d7327, 0x310606, 0x1556ca, 0x73a8c9, 0x60e27b, 0xc08c6b,
};

const Dp half = 0x3fe0000000000000ull;
const Dp two24 = 0x4170000000000000ull;
const Dp invpio2 = 0x3fe45f306dc9c883ull;
const Dp pio2_1 = 0x3ff921fb54400000ull;
const Dp pio2_1t = 0x3dd0b4611a626331ull;
const Dp pio2_2 = 0x3dd0b4611a600000ull;
const Dp pio2_2t = 0x3ba3198a2e037072ull;
const Dp pio2_3 = 0x3ba3198a2e000000ull;
const Dp pio2_3t = 0x397b839a252049c0ull;

static int exponent_of(Dp value) { return static_cast<int>(value >> 52) & 0x7ff; }
}

int __ieee754_rem_pio2(Dp x, Dp* y) {
    const int hx = static_cast<int>(x >> 32);
    const int ix = hx & 0x7fffffff;
    if (ix <= 0x3fe921fb) {
        y[0] = x;
        y[1] = 0;
        return 0;
    }
    if (ix < 0x4002d97c) {
        if (hx > 0) {
            Dp z = dpsub(x, pio2_1);
            Dp tail;
            if (ix != 0x3ff921fb) {
                tail = pio2_1t;
            } else {
                z = dpsub(z, pio2_2);
                tail = pio2_2t;
            }
            y[0] = dpsub(z, tail);
            y[1] = dpsub(dpsub(z, y[0]), tail);
            return 1;
        }
        Dp z = dpadd(x, pio2_1);
        Dp tail;
        if (ix != 0x3ff921fb) {
            tail = pio2_1t;
        } else {
            z = dpadd(z, pio2_2);
            tail = pio2_2t;
        }
        y[0] = dpadd(z, tail);
        y[1] = dpadd(dpsub(z, y[0]), tail);
        return -1;
    }
    if (ix <= 0x413921fb) {
        const Dp t = fabs(x);
        const int n = dptoli(dpadd(dpmul(t, invpio2), half));
        const Dp fn = litodp(n);
        Dp r = dpsub(t, dpmul(fn, pio2_1));
        Dp w = dpmul(fn, pio2_1t);
        if (n < 32 && ix != npio2_hw[n - 1]) {
            y[0] = dpsub(r, w);
        } else {
            const int j = ix >> 20;
            y[0] = dpsub(r, w);
            if (j - exponent_of(y[0]) > 16) {
                const Dp t2 = r;
                w = dpmul(fn, pio2_2);
                r = dpsub(t2, w);
                const Dp low = dpmul(fn, pio2_2t);
                w = dpsub(low, dpsub(dpsub(t2, r), w));
                y[0] = dpsub(r, w);
                if (j - exponent_of(y[0]) > 49) {
                    const Dp t3 = r;
                    w = dpmul(fn, pio2_3);
                    r = dpsub(t3, w);
                    const Dp low3 = dpmul(fn, pio2_3t);
                    w = dpsub(low3, dpsub(dpsub(t3, r), w));
                    y[0] = dpsub(r, w);
                }
            }
        }
        const Dp head = y[0];
        const Dp tail = dpsub(dpsub(r, head), w);
        y[1] = tail;
        if (hx >= 0) return n;
        y[0] = dpsub(0, head);
        y[1] = dpsub(0, tail);
        return -n;
    }
    if (ix >= 0x7ff00000) {
        y[0] = y[1] = dpsub(x, x);
        return 0;
    }
    const int e0 = (ix >> 20) - 1046;
    Dp tx[3];
    Dp z = (static_cast<Dp>(static_cast<unsigned>(ix - (e0 << 20))) << 32) | (x & 0xffffffffull);
    for (int i = 0; i < 2; ++i) {
        tx[i] = litodp(dptoli(z));
        z = dpmul(dpsub(z, tx[i]), two24);
    }
    tx[2] = z;
    int nx = 3;
    while (dpcmp(tx[nx - 1], 0) == 0) --nx;
    const int n = __kernel_rem_pio2(tx, y, e0, nx, 2, two_over_pi);
    if (hx >= 0) return n;
    y[0] = dpsub(0, y[0]);
    y[1] = dpsub(0, y[1]);
    return -n;
}
}
