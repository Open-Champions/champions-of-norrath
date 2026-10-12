// Host check only; not an EE build input. Build the source with -Dtanf=w8_tanf.
// The two callees are stubs that record their arguments.
#include <cstdio>
#include <cstring>
static float k_x, k_y; static int k_sel, k_calls, r_calls; static float r_x;
static int r_n = 0; static float r_y0 = 0.25f, r_y1 = 0.5f;
extern "C" float __kernel_tanf(float x, float y, int s) { ++k_calls; k_x = x; k_y = y; k_sel = s; return 123.0f; }
extern "C" int __ieee754_rem_pio2f(float x, float* y) { ++r_calls; r_x = x; y[0] = r_y0; y[1] = r_y1; return r_n; }
extern "C" float w8_tanf(float);
static float from_bits(unsigned b) { float f; std::memcpy(&f, &b, 4); return f; }
int main() {
    // Boundary: 0x3f490fda stays on the direct path, 0x3f490fdb takes the reduction.
    if (w8_tanf(from_bits(0x3f490fdau)) != 123.0f) return 1;
    if (r_calls != 0 || k_calls != 1 || k_x != from_bits(0x3f490fdau) || k_y != 0.0f || k_sel != 1) return 2;
    if (w8_tanf(from_bits(0xbf490fdau)) != 123.0f) return 3;           // sign ignored for the test
    if (r_calls != 0 || k_x != from_bits(0xbf490fdau) || k_sel != 1) return 4;
    if (w8_tanf(0.0f) != 123.0f || w8_tanf(from_bits(0x80000000u)) != 123.0f || r_calls) return 5;
    r_n = 0;
    w8_tanf(from_bits(0x3f490fdbu));
    if (r_calls != 1 || r_x != from_bits(0x3f490fdbu)) return 6;
    if (k_x != 0.25f || k_y != 0.5f || k_sel != 1) return 7;           // even quadrant: 1
    r_n = 1; w8_tanf(2.0f);
    if (k_sel != -1) return 8;                                          // odd quadrant: -1
    r_n = 2; w8_tanf(-3.0f);
    if (k_sel != 1) return 9;
    r_n = -1; w8_tanf(100.0f);
    if (k_sel != -1) return 10;                                         // negative odd count
    r_n = 7; w8_tanf(1e10f);
    if (k_sel != -1) return 11;
    // Non-finite arguments fall into the reduction path.
    int before = r_calls;
    w8_tanf(from_bits(0x7f800000u));
    if (r_calls != before + 1) return 12;
    std::puts("tanf ok");
    return 0;
}
