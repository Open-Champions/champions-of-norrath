// Host comparison only. Not an EE build input. The callees are scripted.
#include <cstdio>
static unsigned long long seen_value, scripted_fraction;
static int made_cls, made_sign, made_exp; static unsigned made_fraction;
extern "C" {
struct W9UnpackedDouble;
}
#include "dptofp.cpp"
extern "C" void __unpack_d(const unsigned long long* v, W9UnpackedDouble* u) {
    seen_value = *v; u->cls = 4; u->sign = 1; u->exponent = 7; u->fraction = scripted_fraction;
}
extern "C" float __make_fp(int c, int s, int e, unsigned f) { made_cls = c; made_sign = s; made_exp = e; made_fraction = f; return 1.5f; }
int main() {
    int failures = 0;
    struct Case { unsigned long long fraction; unsigned long long want; } cases[] = {
        {0x4000000000000000ull, 0x4000000000000000ull >> 30},           // no low bits: no sticky
        {0x4000000000000001ull, (0x4000000000000001ull >> 30) | 1ull},    // lowest bit set
        {0x400000003fffffffull, (0x400000003fffffffull >> 30) | 1ull},    // highest low bit set
        {0x4000000040000000ull, (0x4000000040000000ull >> 30)}, // bit 30 is data, not sticky
        {0x7fffffffffffffffull, (0x7fffffffffffffffull >> 30) | 1ull},
        {0ull, 0ull},
    };
    for (auto& c : cases) {
        scripted_fraction = c.fraction;
        float r = dptofp(0x0123456789abcdefull);
        if (seen_value != 0x0123456789abcdefull) { std::printf("argument not unpacked\n"); ++failures; }
        if (made_cls != 4 || made_sign != 1 || made_exp != 7) { std::printf("fields wrong\n"); ++failures; }
        if (made_fraction != static_cast<unsigned>(c.want)) { std::printf("fraction %llx -> %x want %llx\n", c.fraction, made_fraction, c.want); ++failures; }
        if (r != 1.5f) { std::printf("result must pass through\n"); ++failures; }
    }
    if (failures) return 2;
    std::printf("dptofp cases passed\n");
    return 0;
}
