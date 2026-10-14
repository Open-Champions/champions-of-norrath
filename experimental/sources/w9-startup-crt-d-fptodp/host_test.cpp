// Host comparison only. Not an EE build input. The callees are scripted to
// record exactly what the wrapper hands them.
#include <cstdio>
static float seen_value;
static int unpack_calls, make_calls;
static int made_cls, made_sign, made_exp;
static unsigned long long made_fraction;
extern "C" {
struct W9UnpackedFloat;
}
#include "fptodp.cpp"
extern "C" void __unpack_f(const float* v, W9UnpackedFloat* u) {
    ++unpack_calls; seen_value = *v;
    u->cls = 3; u->sign = 1; u->exponent = -5; u->fraction = 0x40123457u;
}
extern "C" unsigned long long __make_dp(int c, int s, int e, unsigned long long f) {
    ++make_calls; made_cls = c; made_sign = s; made_exp = e; made_fraction = f; return 0x1122334455667788ull;
}
int main() {
    unsigned long long r = fptodp(2.5f);
    if (seen_value != 2.5f || unpack_calls != 1) { std::printf("unpack argument wrong\n"); return 1; }
    if (make_calls != 1 || made_cls != 3 || made_sign != 1 || made_exp != -5) { std::printf("make fields wrong\n"); return 2; }
    if (made_fraction != (0x40123457ull << 30)) { std::printf("fraction must be shifted left 30 in 64 bits\n"); return 3; }
    if (r != 0x1122334455667788ull) { std::printf("result must pass through\n"); return 4; }
    std::printf("fptodp cases passed\n");
    return 0;
}
