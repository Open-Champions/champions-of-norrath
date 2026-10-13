// Observed __kernel_rem_pio2f at 0x002f6d90, 2148 bytes. Single-precision argument reduction
// by pi/2 for large arguments, called by __ieee754_rem_pio2f (0x002f5760) as
//   n = __kernel_rem_pio2f(x, y, e0, nx, prec, ipio2)
// where x holds nx scaled pieces of the argument, e0 is the binary exponent of its first piece,
// prec selects 0 (one result word in y), 1 or 2 (two words) or 3 (three words), and ipio2 is a
// table of 8-bit-per-word digits of 2/pi. The result is the low three bits of the quotient;
// y receives the remainder, negated when the reduced value came out above pi/4.
// The algorithm follows the long-published scheme for this routine (the structure observed here
// matches it step for step); the lines below restate what the image does:
//   * init_jk (three words 4, 7, 9 at 0x0047d968) gives jk for prec 0..2; prec 3 would read the
//     zero padding word after it (jk = 0), which no caller in the image does. PIo2 (eleven floats at 0x0047d978) is the pi/2 split into 8-bit pieces.
//   * jv = max(0, (e0 - 3) / 8) (the division rounds toward zero as the image does for negative
//     values by adding 7 first); q0 = e0 - 8 * (jv + 1).
//   * f[i] = ipio2[jv - jx + i] as a float, or 0 where that index is negative, for i = 0..jx+jk;
//     q[i] = sum over j of x[j] * f[jx + i - j] for i = 0..jk.
//   * The recompute step splits q into 8-bit integer digits iq[] from the top, scales the
//     leftover with scalbnf by q0, removes multiples of 8 with floorf, and takes n and the
//     fraction; the carry/complement handling for ih (1 or 2) follows, then the all-zero
//     check which either extends the digit table (goto recompute with a larger jz) or
//     shrinks jz, and the final conversion of the digits back to floats.
//   * fq[jz - i] = sum of PIo2[k] * q[i + k] for k up to min(jk, jz - i); the output stage
//     sums fq according to prec.
// Calls scalbnf and floorf. Constants in hex float bits: 0x3b800000 (2^-8), 0x43800000 (2^8),
// 0x3e000000 (0.125), 0x41000000 (8), 0x3f000000 (0.5), 0x3f800000 (1).
// Unknown: the origin of the PIo2 padding entries beyond the first ten; the function is never
// observed with prec 3 in this image's callers. Nothing was executed on the EE.
extern "C" {
float scalbnf(float value, int exponent);
float floorf(float value);

static const int init_jk[4] = {4, 7, 9, 0};  // the fourth word is the zero padding that follows the table in the image
// Bit patterns of the eleven PIo2 floats, as read from the image.
#define NORRATH_FLOAT_BITS(bits) __builtin_bit_cast(float, static_cast<unsigned>(bits))
static const float PIo2[11] = {NORRATH_FLOAT_BITS(0x3fc90000u), NORRATH_FLOAT_BITS(0x39f00000u), NORRATH_FLOAT_BITS(0x37d9ffffu),
                               NORRATH_FLOAT_BITS(0x33a20000u), NORRATH_FLOAT_BITS(0x2e83ffffu), NORRATH_FLOAT_BITS(0x2b4fffffu),
                               NORRATH_FLOAT_BITS(0x27c1ffffu), NORRATH_FLOAT_BITS(0x22cfffffu), NORRATH_FLOAT_BITS(0x1fc40000u),
                               NORRATH_FLOAT_BITS(0x1bc5ffffu), NORRATH_FLOAT_BITS(0x17440000u)};

int __kernel_rem_pio2f(float* x, float* y, int e0, int nx, int prec, const int* ipio2) {
    int iq[20];
    float f[20];
    float q[20];
    float fq[20];
    const float two8 = NORRATH_FLOAT_BITS(0x43800000u);
    const float twon8 = NORRATH_FLOAT_BITS(0x3b800000u);
    const float one = NORRATH_FLOAT_BITS(0x3f800000u);

    const int jk = init_jk[prec];
    const int jx = nx - 1;
    int jv = (e0 - 3 < 0 ? e0 + 4 : e0 - 3) >> 3;
    if (jv < 0) jv = 0;
    int q0 = e0 - 8 * (jv + 1);

    int j = jv - jx;
    const int m = jx + jk;
    for (int i = 0; i <= m; ++i, ++j) f[i] = j < 0 ? 0.0f : static_cast<float>(ipio2[j]);
    for (int i = 0; i <= jk; ++i) {
        float fw = 0.0f;
        for (int k = 0; k <= jx; ++k) fw += x[k] * f[jx + i - k];
        q[i] = fw;
    }

    int jz = jk;
    float z;
    int n, ih;
    for (;;) {
        z = q[jz];
        for (int i = 0, k = jz; k > 0; ++i, --k) {
            float fw = static_cast<float>(static_cast<int>(twon8 * z));
            iq[i] = static_cast<int>(z - two8 * fw);
            z = q[k - 1] + fw;
        }
        z = scalbnf(z, q0);
        z -= 8.0f * floorf(z * 0.125f);
        n = static_cast<int>(z);
        z -= static_cast<float>(n);
        ih = 0;
        if (q0 > 0) {
            int i = iq[jz - 1] >> (8 - q0);
            n += i;
            iq[jz - 1] -= i << (8 - q0);
            ih = iq[jz - 1] >> (7 - q0);
        } else if (q0 == 0) {
            ih = iq[jz - 1] >> 8;
        } else if (z >= 0.5f) {
            ih = 2;
        }
        if (ih > 0) {
            n += 1;
            int carry = 0;
            for (int i = 0; i < jz; ++i) {
                int digit = iq[i];
                if (carry == 0) {
                    if (digit != 0) {
                        carry = 1;
                        iq[i] = 0x100 - digit;
                    }
                } else {
                    iq[i] = 0xff - digit;
                }
            }
            if (q0 > 0) {
                if (q0 == 1) iq[jz - 1] &= 0x7f;
                else if (q0 == 2) iq[jz - 1] &= 0x3f;
            }
            if (ih == 2) {
                z = one - z;
                if (carry != 0) z -= scalbnf(one, q0);
            }
        }
        if (z != 0.0f) break;
        int any = 0;
        for (int i = jz - 1; i >= jk; --i) any |= iq[i];
        if (any != 0) break;
        int k = 1;
        while (iq[jk - k] == 0) ++k;
        for (int i = jz + 1; i <= jz + k; ++i) {
            f[jx + i] = static_cast<float>(ipio2[jv + i]);
            float fw = 0.0f;
            for (int t = 0; t <= jx; ++t) fw += x[t] * f[jx + i - t];
            q[i] = fw;
        }
        jz += k;
    }

    if (z == 0.0f) {
        --jz;
        q0 -= 8;
        while (iq[jz] == 0) {
            --jz;
            q0 -= 8;
        }
    } else {
        z = scalbnf(z, -q0);
        if (z >= two8) {
            float fw = static_cast<float>(static_cast<int>(twon8 * z));
            iq[jz] = static_cast<int>(z - two8 * fw);
            ++jz;
            q0 += 8;
            iq[jz] = static_cast<int>(fw);
        } else {
            iq[jz] = static_cast<int>(z);
        }
    }

    float fw = scalbnf(one, q0);
    for (int i = jz; i >= 0; --i) {
        q[i] = fw * static_cast<float>(iq[i]);
        fw *= twon8;
    }
    for (int i = jz; i >= 0; --i) {
        fw = 0.0f;
        for (int k = 0; k <= jk && k <= jz - i; ++k) fw += PIo2[k] * q[i + k];
        fq[jz - i] = fw;
    }

    if (prec == 0) {
        fw = 0.0f;
        for (int i = jz; i >= 0; --i) fw += fq[i];
        y[0] = ih == 0 ? fw : -fw;
    } else if (prec == 1 || prec == 2) {
        fw = 0.0f;
        for (int i = jz; i >= 0; --i) fw += fq[i];
        y[0] = ih == 0 ? fw : -fw;
        fw = fq[0] - fw;
        for (int i = 1; i <= jz; ++i) fw += fq[i];
        y[1] = ih == 0 ? fw : -fw;
    } else if (prec == 3) {
        for (int i = jz; i > 0; --i) {
            fw = fq[i - 1] + fq[i];
            fq[i] += fq[i - 1] - fw;
            fq[i - 1] = fw;
        }
        for (int i = jz; i > 1; --i) {
            fw = fq[i - 1] + fq[i];
            fq[i] += fq[i - 1] - fw;
            fq[i - 1] = fw;
        }
        fw = 0.0f;
        for (int i = jz; i >= 2; --i) fw += fq[i];
        if (ih == 0) {
            y[0] = fq[0];
            y[1] = fq[1];
            y[2] = fw;
        } else {
            y[0] = -fq[0];
            y[1] = -fq[1];
            y[2] = -fw;
        }
    }
    return n & 7;
}
}
