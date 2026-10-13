// Observed __kernel_rem_pio2 at 0x002f5eb8, 2892 bytes, the double-precision argument reduction by pi/2 for large
// arguments, called by __ieee754_rem_pio2 (0x002f3c78) as
//   n = __kernel_rem_pio2(tx, y, e0, nx, 2, two_over_pi)
// x holds nx pieces of the argument scaled to 24 bits each, e0 is the binary exponent of the first piece, prec
// selects the number of result words (0: one in y, 1 or 2: two, 3: three) and ipio2 is a table of 24-bit digits
// of 2/pi. The result is the low three bits of the quotient; y receives the remainder, negated when the reduced
// value came out above pi/4. Every double is one 64-bit value and every operation is one call of the image's
// soft-double helpers (dpadd, dpsub, dpmul, dpcmp, dptoli, litodp) plus scalbn and floor, in the order the code
// makes them. The algorithm follows the long-published scheme for this routine (the structure observed here
// matches it step for step; the original library routine carries the Sun Microsystems fdlibm notice that
// permits free use with the notice preserved); the lines below restate what the image does:
//   * init_jk (words 2, 3, 4, 6 at 0x0047d868) gives jk = jp for prec 0..3. PIo2 (eight doubles at 0x0047d878)
//     is pi/2 split into 24-bit pieces.
//   * jx = nx - 1; jv = max(0, (e0 - 3) / 24) (a truncating division); q0 = e0 - 24 * (jv + 1).
//   * f[i] = ipio2[jv - jx + i] as a double, or 0 where that index is negative, for i = 0..jx+jk;
//     q[i] = sum over j of x[j] * f[jx + i - j] for i = 0..jk.
//   * recompute: q[jz..] is distilled from the top into 24-bit integer digits iq[] (each step takes
//     (int)(z * 2^-24) and the remainder), the leftover z is scaled by q0 with scalbn, multiples of 8 are removed
//     with floor, n and the fraction follow, ih (0, 1 or 2) records whether the fraction is above 1/2, and for a
//     positive ih the digits are complemented with carry (with a mask for q0 of 1 or 2). If the fraction is zero
//     and the low digits are zero the table is extended (goto recompute with a larger jz); a zero fraction
//     otherwise shrinks jz past zero digits, and a nonzero one is rescaled and split into one or two digits.
//   * the digits are turned back into doubles (q[i] = fw * iq[i], fw shrinking by 2^-24), then
//     fq[jz - i] = sum of PIo2[k] * q[i + k] for k up to min(jp, jz - i); the output stage sums fq according to
//     prec, with a compensated pairwise pass for prec 3.
// Constants are written as bit patterns (1.0, 2^24, 2^-24, 0.125, 8.0, 0.5, zero), as the image loads them.
// Unknown: the image reads init_jk and PIo2 with prec as given (callers use 2 only); entries beyond the eight PIo2
// doubles are not read. Nothing was executed on the EE.
extern "C" {
typedef unsigned long long Dp;
Dp dpmul(Dp left, Dp right);
Dp dpadd(Dp left, Dp right);
Dp dpsub(Dp left, Dp right);
int dpcmp(Dp left, Dp right);
int dptoli(Dp value);
Dp litodp(int value);
Dp scalbn(Dp value, int exponent);
Dp floor(Dp value);

namespace {
const int init_jk[4] = {2, 3, 4, 6};
// Bit patterns of the eight PIo2 doubles, as read from the image.
const Dp PIo2[8] = {0x3ff921fb3fffffffull, 0x3e74442cffffffffull, 0x3cf846987fffffffull, 0x3b78cc515fffffffull,
                    0x39f01b837fffffffull, 0x387a25203fffffffull, 0x36e3822280000000ull, 0x3569f31d00000000ull};
const Dp kZero = 0ull;
const Dp kOne = 0x3ff0000000000000ull;
const Dp kTwo24 = 0x4170000000000000ull;
const Dp kTwoN24 = 0x3e70000000000000ull;
const Dp kEighth = 0x3fc0000000000000ull;
const Dp kEight = 0x4020000000000000ull;
const Dp kHalf = 0x3fe0000000000000ull;
}  // namespace

int __kernel_rem_pio2(Dp* x, Dp* y, int e0, int nx, int prec, const int* ipio2) {
    int iq[20];
    Dp f[20];
    Dp fq[20];
    Dp q[20];
    Dp z, fw;
    int i, j, k, ih, n, carry;

    const int jk = init_jk[prec];
    const int jp = jk;
    const int jx = nx - 1;
    int jv = (e0 - 3) / 24;
    if (jv < 0) jv = 0;
    int q0 = e0 - 24 * (jv + 1);

    j = jv - jx;
    const int m = jx + jk;
    for (i = 0; i <= m; ++i, ++j) f[i] = j < 0 ? kZero : litodp(ipio2[j]);
    for (i = 0; i <= jk; ++i) {
        fw = kZero;
        for (j = 0; j <= jx; ++j) {
            Dp product = dpmul(x[j], f[jx + i - j]);
            fw = dpadd(fw, product);
        }
        q[i] = fw;
    }

    int jz = jk;
recompute:
    z = q[jz];
    for (i = 0, j = jz; j > 0; ++i, --j) {
        Dp scaled = dpmul(z, kTwoN24);
        int top = dptoli(scaled);
        fw = litodp(top);
        Dp back = dpmul(fw, kTwo24);
        Dp rest = dpsub(z, back);
        iq[i] = dptoli(rest);
        z = dpadd(q[j - 1], fw);
    }

    z = scalbn(z, q0);
    Dp eighths = dpmul(z, kEighth);
    Dp whole = floor(eighths);
    Dp trim = dpmul(whole, kEight);
    z = dpsub(z, trim);
    n = dptoli(z);
    Dp n_double = litodp(n);
    z = dpsub(z, n_double);
    ih = 0;
    if (q0 > 0) {
        i = iq[jz - 1] >> (24 - q0);
        n += i;
        iq[jz - 1] -= i << (24 - q0);
        ih = iq[jz - 1] >> (23 - q0);
    } else if (q0 == 0) {
        ih = iq[jz - 1] >> 23;
    } else {
        ih = dpcmp(z, kHalf) >= 0 ? 2 : 0;
    }

    if (ih > 0) {
        n += 1;
        carry = 0;
        for (i = 0; i < jz; ++i) {
            j = iq[i];
            if (carry == 0) {
                if (j != 0) {
                    carry = 1;
                    iq[i] = 0x1000000 - j;
                }
            } else {
                iq[i] = 0xffffff - j;
            }
        }
        if (q0 > 0) {
            switch (q0) {
            case 1:
                iq[jz - 1] &= 0x7fffff;
                break;
            case 2:
                iq[jz - 1] &= 0x3fffff;
                break;
            }
        }
        if (ih == 2) {
            z = dpsub(kOne, z);
            if (carry != 0) {
                Dp unit = scalbn(kOne, q0);
                z = dpsub(z, unit);
            }
        }
    }

    if (dpcmp(z, kZero) == 0) {
        j = 0;
        for (i = jz - 1; i >= jk; --i) j |= iq[i];
        if (j == 0) {
            for (k = 1; iq[jk - k] == 0; ++k) {
            }
            for (i = jz + 1; i <= jz + k; ++i) {
                f[jx + i] = litodp(ipio2[jv + i]);
                fw = kZero;
                for (j = 0; j <= jx; ++j) {
                    Dp product = dpmul(x[j], f[jx + i - j]);
                    fw = dpadd(fw, product);
                }
                q[i] = fw;
            }
            jz += k;
            goto recompute;
        }
    }

    if (dpcmp(z, kZero) == 0) {
        jz -= 1;
        q0 -= 24;
        while (iq[jz] == 0) {
            jz--;
            q0 -= 24;
        }
    } else {
        z = scalbn(z, -q0);
        if (dpcmp(z, kTwo24) >= 0) {
            q0 += 24;
            Dp scaled = dpmul(z, kTwoN24);
            int top = dptoli(scaled);
            fw = litodp(top);
            Dp back = dpmul(fw, kTwo24);
            Dp rest = dpsub(z, back);
            iq[jz] = dptoli(rest);
            jz += 1;
            iq[jz] = dptoli(fw);
        } else {
            iq[jz] = dptoli(z);
        }
    }

    fw = scalbn(kOne, q0);
    for (i = jz; i >= 0; --i) {
        Dp digit = litodp(iq[i]);
        q[i] = dpmul(fw, digit);
        fw = dpmul(fw, kTwoN24);
    }

    for (i = jz; i >= 0; --i) {
        fw = kZero;
        for (k = 0; k <= jp && k <= jz - i; ++k) {
            Dp product = dpmul(PIo2[k], q[i + k]);
            fw = dpadd(fw, product);
        }
        fq[jz - i] = fw;
    }

    switch (prec) {
    case 0:
        fw = kZero;
        for (i = jz; i >= 0; --i) fw = dpadd(fw, fq[i]);
        y[0] = ih == 0 ? fw : dpsub(kZero, fw);
        break;
    case 1:
    case 2:
        fw = kZero;
        for (i = jz; i >= 0; --i) fw = dpadd(fw, fq[i]);
        y[0] = ih == 0 ? fw : dpsub(kZero, fw);
        fw = dpsub(fq[0], fw);
        for (i = 1; i <= jz; ++i) fw = dpadd(fw, fq[i]);
        y[1] = ih == 0 ? fw : dpsub(kZero, fw);
        break;
    case 3:
        for (i = jz; i > 0; --i) {
            Dp upper = fq[i];
            Dp lower = fq[i - 1];
            fw = dpadd(lower, upper);
            Dp error = dpsub(lower, fw);
            fq[i] = dpadd(upper, error);
            fq[i - 1] = fw;
        }
        for (i = jz; i > 1; --i) {
            Dp upper = fq[i];
            Dp lower = fq[i - 1];
            fw = dpadd(lower, upper);
            Dp error = dpsub(lower, fw);
            fq[i] = dpadd(upper, error);
            fq[i - 1] = fw;
        }
        fw = kZero;
        for (i = jz; i >= 2; --i) fw = dpadd(fw, fq[i]);
        if (ih == 0) {
            y[0] = fq[0];
            y[1] = fq[1];
            y[2] = fw;
        } else {
            y[0] = dpsub(kZero, fq[0]);
            y[1] = dpsub(kZero, fq[1]);
            y[2] = dpsub(kZero, fw);
        }
        break;
    }
    return n & 7;
}
}
