// Host contract for __extendsfdf2. Not an EE plan input. fptodp is modelled with the host's own
// conversion and records the argument it received.
#include <cstdio>
#include <cstring>
#include "extendsfdf2.cpp"
static int calls; static float seen;
extern "C" unsigned long long fptodp(float value) {
    ++calls; seen = value;
    const double d = value; unsigned long long b; std::memcpy(&b, &d, 8); return b;
}
int main() {
    const float v[] = {0.0f, -0.0f, 1.0f, -2.5f, 3.4028235e38f, 1.17549435e-38f, 1e-45f};
    for (float x : v) {
        const double d = x; unsigned long long want; std::memcpy(&want, &d, 8);
        if (__extendsfdf2(x) != want || std::memcmp(&seen, &x, 4) != 0) return 1;
    }
    if (calls != 7) return 2;
    std::puts("__extendsfdf2 ok");
    return 0;
}
