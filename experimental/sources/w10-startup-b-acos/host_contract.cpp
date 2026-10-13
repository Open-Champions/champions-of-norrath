#include <cstdio>
static double seen;
static int calls;
extern "C" double __ieee754_acos(double x) { ++calls; seen = x; return x * 3 + 1; }
#include "acos.cpp"
int main() {
    const double values[] = {0, 1, -1, 0.5, 123.25, -77.75};
    for (double v : values) {
        calls = 0;
        const double r = acos(v);
        if (calls != 1 || seen != v || r != v * 3 + 1) { std::printf("acos wrong for %g\n", (double)v); return 1; }
    }
    std::puts("acos forwards its argument and result");
    return 0;
}
