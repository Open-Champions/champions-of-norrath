// Host check only; not an EE build input. Build the source with -Dsinf=w12_sinf.
// The three callees are stubs that record their arguments.
#include <cstdio>
#include <cstring>
static float s_x, s_y; static int s_sel, s_calls, c_calls, r_calls; static float c_x, c_y, r_x;
static int r_n = 0; static float r_y0 = 0.25f, r_y1 = 0.5f;
extern "C" float __kernel_sinf(float x, float y, int s) { ++s_calls; s_x = x; s_y = y; s_sel = s; return 3.0f; }
extern "C" float __kernel_cosf(float x, float y) { ++c_calls; c_x = x; c_y = y; return 5.0f; }
extern "C" int __ieee754_rem_pio2f(float x, float* y) { ++r_calls; r_x = x; y[0] = r_y0; y[1] = r_y1; return r_n; }
extern "C" float w12_sinf(float);
static float from_bits(unsigned b) { float f; std::memcpy(&f, &b, 4); return f; }
int main() {
    if (w12_sinf(from_bits(0x3f490fdau)) != 3.0f) return 1;
    if (r_calls || s_calls != 1 || s_x != from_bits(0x3f490fdau) || s_y != 0.0f || s_sel != 0) return 2;
    if (w12_sinf(from_bits(0xbf490fdau)) != 3.0f || r_calls || s_x != from_bits(0xbf490fdau)) return 3;
    if (w12_sinf(0.0f) != 3.0f || w12_sinf(from_bits(0x80000000u)) != 3.0f || r_calls) return 4;
    r_n = 0; if (w12_sinf(from_bits(0x3f490fdbu)) != 3.0f) return 5;
    if (r_calls != 1 || r_x != from_bits(0x3f490fdbu) || s_x != 0.25f || s_y != 0.5f || s_sel != 1) return 6;
    int sc = s_calls, cc = c_calls;
    r_n = 1; if (w12_sinf(2.0f) != 5.0f) return 7;
    if (c_x != 0.25f || c_y != 0.5f || s_calls != sc || c_calls != cc + 1) return 8;
    r_n = 2; if (w12_sinf(-3.0f) != -3.0f) return 9;
    if (s_sel != 1 || s_x != 0.25f) return 10;
    r_n = 3; if (w12_sinf(100.0f) != -5.0f) return 11;
    r_n = 4; if (w12_sinf(100.0f) != 3.0f) return 12;
    r_n = -1; if (w12_sinf(100.0f) != -5.0f) return 13;   // -1 & 3 == 3
    r_n = -2; if (w12_sinf(100.0f) != -3.0f) return 14;
    int before = r_calls; r_n = 0;
    w12_sinf(from_bits(0x7f800000u));
    if (r_calls != before + 1) return 15;
    std::puts("sinf ok");
    return 0;
}
