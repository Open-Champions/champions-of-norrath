#include <cstdio>
static float seen_x, seen_y;
static int calls;
extern "C" float __ieee754_fmodf(float x, float y) { ++calls; seen_x = x; seen_y = y; return x * 3 + y; }
#include "fmodf.cpp"
int main() {
    const float values[] = {0, 1, -1, 0.5f, 123.25f, -77.75f};
    for (float a : values) for (float b : values) {
        calls = 0;
        const float r = fmodf(a, b);
        if (calls != 1 || seen_x != a || seen_y != b || r != a * 3 + b) { std::printf("fmodf wrong for %g %g\n", a, b); return 1; }
    }
    std::puts("fmodf forwards its arguments in order and its result");
    return 0;
}
