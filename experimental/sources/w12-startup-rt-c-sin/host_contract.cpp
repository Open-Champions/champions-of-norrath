// Host check only; not an EE build input. Build the source with -Dsin=w12_sin.
#include <cstdio>
#include <cstring>
typedef unsigned long long Dp;
extern "C" {
static int calls[8]; static Dp ka, kb; static int ksel; static int rn; static Dp ra, d0, d1;
static Dp rem0 = 0x1111, rem1 = 0x2222;
Dp __kernel_sin(Dp x, Dp y, int s) { ++calls[0]; ka = x; kb = y; ksel = s; return 0x5100; }
Dp __kernel_cos(Dp x, Dp y) { ++calls[1]; ka = x; kb = y; return 0xc050; }
int __ieee754_rem_pio2(Dp x, Dp* y) { ++calls[2]; ra = x; y[0] = rem0; y[1] = rem1; return rn; }
Dp dpsub(Dp a, Dp b) { ++calls[3]; d0 = a; d1 = b; return 0xd000 + (b & 0xff); }
Dp w12_sin(Dp);
}
int main() {
    if (w12_sin(0x3fe921fb00000000ull) != 0x5100 || calls[2] || ka != 0x3fe921fb00000000ull || kb != 0 || ksel != 0) return 1;
    if (w12_sin(0xbfe921fbffffffffull) != 0x5100 || calls[2] || ksel != 0) return 2;      // sign ignored, low word ignored
    if (w12_sin(0) != 0x5100 || w12_sin(0x8000000000000000ull) != 0x5100 || calls[2]) return 3;
    rn = 0;
    if (w12_sin(0x3fe921fc00000000ull) != 0x5100 || calls[2] != 1 || ra != 0x3fe921fc00000000ull) return 4;
    if (ka != rem0 || kb != rem1 || ksel != 1) return 5;
    rn = 1; if (w12_sin(0x4000000000000000ull) != 0xc050 || ka != rem0 || kb != rem1) return 6;
    rn = 2; if (w12_sin(0x4000000000000000ull) != 0xd000 + 0 || d0 != 0 || d1 != 0x5100 || ksel != 1) return 7;
    rn = 3; if (w12_sin(0x4000000000000000ull) != 0xd000 + 0x50 || d0 != 0 || d1 != 0xc050) return 8;
    rn = 4; if (w12_sin(0x4000000000000000ull) != 0x5100) return 9;
    rn = -1; if (w12_sin(0x4000000000000000ull) != 0xd000 + 0x50) return 10;           // -1 & 3 == 3
    int before = calls[2]; calls[3] = 0;
    if (w12_sin(0x7ff0000000000000ull) != 0xd000 + 0x00 || d0 != 0x7ff0000000000000ull || d1 != 0x7ff0000000000000ull) return 11;
    if (calls[2] != before || calls[3] != 1) return 12;                                // infinity: no reduction
    if (w12_sin(0xfff8000000000001ull) == 0 || calls[2] != before) return 13;           // NaN, negative
    before = calls[2]; calls[3] = 0;
    w12_sin(0x7fefffffffffffffull);                                                    // largest finite: reduced
    if (calls[2] != before + 1) return 14;
    std::puts("sin ok");
    return 0;
}
