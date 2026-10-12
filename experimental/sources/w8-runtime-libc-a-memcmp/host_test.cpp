// Host comparison only. Not an EE build input.
#include <cstdio>
#include <cstring>
#include <initializer_list>

#define memcmp w8_memcmp_under_test
#include "memcmp.cpp"
#undef memcmp

static int sign(int v) { return (v > 0) - (v < 0); }

static int check(const void* a, const void* b, unsigned n, int line) {
    const unsigned char* x = static_cast<const unsigned char*>(a);
    const unsigned char* y = static_cast<const unsigned char*>(b);
    int expect = 0;
    for (unsigned i = 0; i < n; ++i) if (x[i] != y[i]) { expect = int(x[i]) - int(y[i]); break; }
    const int got = w8_memcmp_under_test(a, b, n);
    if (got != expect || sign(got) != sign(std::memcmp(a, b, n))) {
        std::printf("memcmp case %d got %d expect %d\n", line, got, expect);
        return 1;
    }
    return 0;
}

int main() {
    alignas(16) unsigned char left[96];
    alignas(16) unsigned char right[96];
    int failures = 0;
    for (int i = 0; i < 96; ++i) left[i] = right[i] = static_cast<unsigned char>(i * 7 + 3);
    for (unsigned n = 0; n <= 96; ++n) failures += check(left, right, n, 1);
    for (unsigned at = 0; at < 96; ++at) {
        const unsigned char saved = right[at];
        right[at] = static_cast<unsigned char>(saved ^ 0x80);   // crosses the sign boundary
        for (unsigned n : {0u, 1u, at, at + 1u, 96u}) if (n <= 96) failures += check(left, right, n, 2) + check(right, left, n, 3);
        right[at] = static_cast<unsigned char>(saved + 1);
        failures += check(left, right, 96, 4) + check(left + 1, right + 1, 90, 5);
        right[at] = saved;
    }
    unsigned char high[2] = {0xff, 0}, low[2] = {0x00, 0};
    failures += check(high, low, 2, 6) + check(low, high, 2, 7);
    if (failures) return 1;
    std::printf("memcmp cases passed\n");
    return 0;
}
