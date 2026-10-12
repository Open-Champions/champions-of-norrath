#include "eval_hermite.cpp"
#include <cstdio>
static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
int main() {
    // Cubic Hermite endpoints: value at t = 0 is start, at t = 1 is end.
    expect(evalHermite__Ffffff(0.0f, 3.0f, 9.0f, 5.0f, 7.0f) == 3.0f, "t=0 gives start");
    expect(evalHermite__Ffffff(1.0f, 3.0f, 9.0f, 5.0f, 7.0f) == 9.0f, "t=1 gives end");
    // t = 0.5 with zero tangents is the midpoint; the tangents add (m0 - m1) / 8.
    expect(evalHermite__Ffffff(0.5f, 2.0f, 6.0f, 0.0f, 0.0f) == 4.0f, "midpoint without tangents");
    expect(evalHermite__Ffffff(0.5f, 0.0f, 0.0f, 8.0f, 0.0f) == 1.0f, "start tangent weight 1/8");
    expect(evalHermite__Ffffff(0.5f, 0.0f, 0.0f, 0.0f, 8.0f) == -1.0f, "end tangent weight -1/8");
    return failures;
}
