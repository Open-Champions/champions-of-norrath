// Host comparison only. Not an EE build input. __unpack_d tags the record with
// its input; _fpadd_parts records what it received and hands back the scratch.
#include <cstdio>
extern "C" {
struct W9UnpackedDouble;
}
#include "dpsub.cpp"
extern "C" void __unpack_d(const unsigned long long* v, W9UnpackedDouble* u) { u->cls = 1; u->sign = static_cast<int>(*v & 1); u->fraction = *v; }
static W9UnpackedDouble *got_a, *got_b, *got_scratch;
static int a_sign, b_sign;
extern "C" W9UnpackedDouble* _fpadd_parts(W9UnpackedDouble* a, W9UnpackedDouble* b, W9UnpackedDouble* s) {
    got_a = a; got_b = b; got_scratch = s; a_sign = a->sign; b_sign = b->sign;
    s->fraction = a->fraction * 1000 + b->fraction;
    return s;
}
extern "C" unsigned long long __pack_d(const W9UnpackedDouble* u) { return u->fraction + 0x1000000000ull; }
int main() {
    int failures = 0;
    unsigned long long r = dpsub(7ull, 4ull);
    if (r != 7004ull + 0x1000000000ull) { std::printf("operand order or result wrong: %llx\n", r); ++failures; }
    if (got_a == got_b || got_a == got_scratch || got_b == got_scratch) { std::printf("records must be distinct\n"); ++failures; }
    if (a_sign != 1) { std::printf("first sign must be untouched\n"); ++failures; }
    if (b_sign != 1) { std::printf("second sign wrong: %d\n", b_sign); ++failures; }
    r = dpsub(2ull, 5ull);
    if (a_sign != 0 || b_sign != 0) { std::printf("signs wrong on second case\n"); ++failures; }
    if (failures) return 2;
    std::printf("dpsub cases passed\n");
    return 0;
}
