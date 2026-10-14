#include <cstdio>
static float seen;
static int calls;
extern "C" float __ieee754_acosf(float x) { ++calls; seen = x; return x * 3 + 1; }
#include "acosf.cpp"
int main() {
    const float values[] = {0, 1, -1, 0.5, 123.25, -77.75};
    for (float v : values) {
        calls = 0;
        const float r = acosf(v);
        if (calls != 1 || seen != v || r != v * 3 + 1) { std::printf("acosf wrong for %g\n", (double)v); return 1; }
    }
    std::puts("acosf forwards its argument and result");
    return 0;
}
