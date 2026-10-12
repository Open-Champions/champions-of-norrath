// Host check only; not an EE build input. Build the source with -Dpowf=w8_powf.
#include <cstdio>
#include <cstring>
static float seen_a, seen_b; static int calls;
extern "C" float __ieee754_powf(float a, float b) { ++calls; seen_a = a; seen_b = b; return a * 10.0f + b; }
extern "C" float w8_powf(float, float);
int main() {
    if (w8_powf(2.0f, 3.0f) != 23.0f) return 1;
    if (calls != 1 || seen_a != 2.0f || seen_b != 3.0f) return 2;  // order preserved
    if (w8_powf(-1.5f, 0.25f) != -14.75f) return 3;
    std::puts("powf ok");
    return 0;
}
