// Observed __ieee754_rem_pio2f at 0x002f5760, 944 bytes: float argument
// reduction, n = __ieee754_rem_pio2f(x, y) with y[0] + y[1] ~ x - n*(pi/2) and
// the result the quadrant count (negative for negative x). Four ranges by
// ix = bits(x) & 0x7fffffff, read from the instruction flow:
//  1. ix <= 0x3f490fd8: y[0] = x, y[1] = 0, return 0.
//  2. ix <= 0x4016cbe3: z = x -/+ pio2_1 (minus for positive x, plus for
//     negative). Unless (ix & 0xfffffff0) == 0x3fc90fd0: y[0] = z -/+ pio2_1t
//     and y[1] = (z - y[0]) -/+ pio2_1t. In that case z is first moved by
//     pio2_2 and pio2_2t is used the same way. Return +1 or -1.
//  3. ix <= 0x43490f80: t = |x|, n = (int)(t*invpio2 + 0.5), fn = (float)n,
//     r = t - fn*pio2_1, w = fn*pio2_1t. When n < 32 and (ix & 0xffffff00)
//     differs from npio2_hw[n-1], y[0] = r - w and no refinement runs. Else
//     y[0] = r - w, then the exponent of y[0] is compared with ix>>23: a gap of
//     more than 8 repeats the step with pio2_2/pio2_2t, and a gap of more than 25
//     after that once more with pio2_3/pio2_3t. y[1] = (r - y[0]) - w. Negative
//     x negates both and returns -n.
//  4. Larger: e0 = (ix>>23) - 134, z = bits(ix - (e0<<23)); two truncations
//     tx[i] = (float)(int)z, z = (z - tx[i]) * 256; tx[2] = z; nx counts down
//     past zero entries from 3; the original routine __kernel_rem_pio2f runs
//     with (tx, y, e0, nx, 2, two_over_pi). Negative x negates y and returns -n.
//     The original has no separate test for infinity or NaN.
// Constants (hex float bits): invpio2 3f22f984, pio2_1 3fc90f80, pio2_1t
// 37354442, pio2_2 373543ff, pio2_2t 2e85a308, pio2_3 2e85a300, pio2_3t
// 248d3131 (the table at 0x0047d438 uses the standard 2/pi byte values; its
// 198 entries and npio2_hw (0x0047d750) were checked against an independent
// computation). Inferred: the standard fdlibm single-float reduction; the
// names are mine. Unknown: the owners of the neighbouring constants.
extern "C" {
float fabsf(float);
int __kernel_rem_pio2f(float* x, float* y, int e0, int nx, int prec, const int* ipio2);
}

namespace {
float from_bits(unsigned bits) { return __builtin_bit_cast(float, bits); }
unsigned to_bits(float value) { return __builtin_bit_cast(unsigned, value); }

const unsigned npio2_hw[32] = {
    0x3fc90f00u, 0x40490f00u, 0x4096cb00u, 0x40c90f00u,
    0x40fb5300u, 0x4116cb00u, 0x412fed00u, 0x41490f00u,
    0x41623100u, 0x417b5300u, 0x418a3a00u, 0x4196cb00u,
    0x41a35c00u, 0x41afed00u, 0x41bc7e00u, 0x41c90f00u,
    0x41d5a000u, 0x41e23100u, 0x41eec200u, 0x41fb5300u,
    0x4203f200u, 0x420a3a00u, 0x42108300u, 0x4216cb00u,
    0x421d1400u, 0x42235c00u, 0x4229a500u, 0x422fed00u,
    0x42363600u, 0x423c7e00u, 0x4242c700u, 0x42490f00u,
};

const int two_over_pi[198] = {
    162, 249, 131, 110, 78, 68, 21, 41, 252, 39, 87, 209, 245, 52, 221, 192,
    219, 98, 149, 153, 60, 67, 144, 65, 254, 81, 99, 171, 222, 187, 197, 97,
    183, 36, 110, 58, 66, 77, 210, 224, 6, 73, 46, 234, 9, 209, 146, 28,
    254, 29, 235, 28, 177, 41, 167, 62, 232, 130, 53, 245, 46, 187, 68, 132,
    233, 156, 112, 38, 180, 95, 126, 65, 57, 145, 214, 57, 131, 83, 57, 244,
    156, 132, 95, 139, 189, 249, 40, 59, 31, 248, 151, 255, 222, 5, 152, 15,
    239, 47, 17, 139, 90, 10, 109, 31, 109, 54, 126, 207, 39, 203, 9, 183,
    79, 70, 63, 102, 158, 95, 234, 45, 117, 39, 186, 199, 235, 229, 241, 123,
    61, 7, 57, 247, 138, 82, 146, 234, 107, 251, 95, 177, 31, 141, 93, 8,
    86, 3, 48, 70, 252, 123, 107, 171, 240, 207, 188, 32, 154, 244, 54, 29,
    169, 227, 145, 97, 94, 230, 27, 8, 101, 153, 133, 95, 20, 160, 104, 64,
    141, 255, 216, 128, 77, 115, 39, 49, 6, 6, 21, 86, 202, 115, 168, 201,
    96, 226, 123, 192, 140, 107,
};
}

extern "C" int __ieee754_rem_pio2f(float x, float* y) {
    const float half = from_bits(0x3f000000u);
    const float two8 = from_bits(0x43800000u);
    const float invpio2 = from_bits(0x3f22f984u);
    const float pio2_1 = from_bits(0x3fc90f80u);
    const float pio2_1t = from_bits(0x37354442u);
    const float pio2_2 = from_bits(0x373543ffu);
    const float pio2_2t = from_bits(0x2e85a308u);
    const float pio2_3 = from_bits(0x2e85a300u);
    const float pio2_3t = from_bits(0x248d3131u);

    const int hx = static_cast<int>(to_bits(x));
    const int ix = hx & 0x7fffffff;
    if (ix <= 0x3f490fd8) {
        y[0] = x;
        y[1] = 0.0f;
        return 0;
    }
    if (ix <= 0x4016cbe3) {
        if (hx > 0) {
            float z = x - pio2_1;
            if ((ix & 0xfffffff0) != 0x3fc90fd0) {
                y[0] = z - pio2_1t;
                y[1] = (z - y[0]) - pio2_1t;
            } else {
                z = z - pio2_2;
                y[0] = z - pio2_2t;
                y[1] = (z - y[0]) - pio2_2t;
            }
            return 1;
        }
        float z = x + pio2_1;
        if ((ix & 0xfffffff0) != 0x3fc90fd0) {
            y[0] = z + pio2_1t;
            y[1] = (z - y[0]) + pio2_1t;
        } else {
            z = z + pio2_2;
            y[0] = z + pio2_2t;
            y[1] = (z - y[0]) + pio2_2t;
        }
        return -1;
    }
    if (ix <= 0x43490f80) {
        const float t = fabsf(x);
        const int n = static_cast<int>(t * invpio2 + half);
        const float fn = static_cast<float>(n);
        float r = t - fn * pio2_1;
        float w = fn * pio2_1t;
        bool refine = true;
        if (n < 32 && static_cast<unsigned>(ix & 0xffffff00) != npio2_hw[n - 1]) {
            y[0] = r - w;
            refine = false;
        } else {
            y[0] = r - w;
        }
        if (refine) {
            const int j = ix >> 23;
            int gap = j - static_cast<int>((to_bits(y[0]) >> 23) & 0xff);
            if (gap > 8) {
                const float t2 = r;
                w = fn * pio2_2;
                r = t2 - w;
                w = fn * pio2_2t - ((t2 - r) - w);
                y[0] = r - w;
                gap = j - static_cast<int>((to_bits(y[0]) >> 23) & 0xff);
                if (gap > 25) {
                    const float t3 = r;
                    w = fn * pio2_3;
                    r = t3 - w;
                    w = fn * pio2_3t - ((t3 - r) - w);
                    y[0] = r - w;
                }
            }
        }
        y[1] = (r - y[0]) - w;
        if (hx < 0) {
            y[0] = -y[0];
            y[1] = -y[1];
            return -n;
        }
        return n;
    }
    const int e0 = (ix >> 23) - 134;
    float z = from_bits(static_cast<unsigned>(ix - (e0 << 23)));
    float tx[3];
    for (int i = 0; i < 2; ++i) {
        tx[i] = static_cast<float>(static_cast<int>(z));
        z = (z - tx[i]) * two8;
    }
    tx[2] = z;
    int nx = 3;
    while (tx[nx - 1] == 0.0f) nx--;
    const int n = __kernel_rem_pio2f(tx, y, e0, nx, 2, two_over_pi);
    if (hx < 0) {
        y[0] = -y[0];
        y[1] = -y[1];
        return -n;
    }
    return n;
}
