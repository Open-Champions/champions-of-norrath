// Host contract for cosf. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// The three helpers are scripted and record their arguments, so the boundary, the dispatch on the
// quadrant, the use of both reduced parts, the iy flag and the sign flips are each pinned.
#include <cstdio>
#include <cstring>
#include <string>
static std::string trace;
static int quadrant;
extern "C" {
float __kernel_cosf(float x, float y) { char b[64]; std::snprintf(b, sizeof b, "C(%g,%g);", x, y); trace += b; return 10.0f; }
float __kernel_sinf(float x, float y, int iy) { char b[64]; std::snprintf(b, sizeof b, "S(%g,%g,%d);", x, y, iy); trace += b; return 20.0f; }
int __ieee754_rem_pio2f(float x, float* y) { char b[64]; std::snprintf(b, sizeof b, "R(%g);", x); trace += b; y[0] = 1.5f; y[1] = 0.25f; return quadrant; }
}
#include "cosf.cpp"
static float bits(unsigned u) { float f; std::memcpy(&f, &u, 4); return f; }
static int expect(float x, int q, float want, const char* want_trace) {
    trace.clear(); quadrant = q;
    const float got = cosf(x);
    if (got != want || trace != want_trace) { std::printf("x=%g q=%d got %g trace %s\n", x, q, got, trace.c_str()); return 1; }
    return 0;
}
int main() {
    int bad = 0;
    bad += expect(0.5f, 9, 10.0f, "C(0.5,0);");                       // no reduction, y = 0
    bad += expect(-0.5f, 9, 10.0f, "C(-0.5,0);");                     // sign bit ignored in the test
    bad += expect(bits(0x3f490fd8), 9, 10.0f, "C(0.785398,0);");       // boundary value: still direct
    bad += expect(bits(0xbf490fd8), 9, 10.0f, "C(-0.785398,0);");
    bad += expect(bits(0x3f490fd9), 0, 10.0f, "R(0.785398);C(1.5,0.25);");   // next float up: reduced
    bad += expect(bits(0xbf490fd9), 0, 10.0f, "R(-0.785398);C(1.5,0.25);");
    bad += expect(3.0f, 4, 10.0f, "R(3);C(1.5,0.25);");                // quadrant 4 behaves like 0
    bad += expect(3.0f, 1, -20.0f, "R(3);S(1.5,0.25,1);");
    bad += expect(3.0f, 5, -20.0f, "R(3);S(1.5,0.25,1);");
    bad += expect(3.0f, 2, -10.0f, "R(3);C(1.5,0.25);");
    bad += expect(3.0f, 3, 20.0f, "R(3);S(1.5,0.25,1);");
    bad += expect(3.0f, -1, 20.0f, "R(3);S(1.5,0.25,1);");             // -1 & 3 == 3
    bad += expect(3.0f, -2, -10.0f, "R(3);C(1.5,0.25);");
    bad += expect(bits(0x7f800000), 0, 10.0f, "R(inf);C(1.5,0.25);");   // infinity goes to the reduction
    return bad;
}
