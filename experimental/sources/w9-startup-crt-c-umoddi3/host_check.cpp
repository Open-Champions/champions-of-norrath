// Host check: compares the word-only routine with the host's native 64-bit remainder over edge
// values and a deterministic pseudo-random sweep covering every divisor/dividend size class.
// A zero divisor is excluded (the routine deliberately traps there).
#include "umoddi3.cpp"
#include <cstdio>

static unsigned long long state = 0x9e3779b97f4a7c15ull;
static unsigned long long next() {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return state;
}

static int check(unsigned long long n, unsigned long long d) {
    if (d == 0) return 0;
    if (__umoddi3(n, d) != n % d) {
        std::printf("mismatch %llx %% %llx\n", n, d);
        return 1;
    }
    return 0;
}

int main() {
    const unsigned long long edge[] = {1ull, 2ull, 3ull, 0xffull, 0x100ull, 0xffffull, 0x10000ull,
                                       0xffffffull, 0x1000000ull, 0x7fffffffull, 0x80000000ull,
                                       0xffffffffull, 0x100000000ull, 0x100000001ull,
                                       0x1ffffffffull, 0x7fffffffffffffffull,
                                       0x8000000000000000ull, 0xffffffff00000000ull,
                                       0xfffffffffffffffeull, 0xffffffffffffffffull, 0ull};
    for (unsigned long long n : edge)
        for (unsigned long long d : edge)
            if (check(n, d)) return 1;
    for (int i = 0; i < 2000000; ++i) {
        unsigned long long n = next(), d = next();
        n >>= next() % 64;
        d >>= next() % 64;
        if (check(n, d)) return 2;
        // dividend near a multiple of the divisor, to hit the correction steps
        if (d != 0) {
            unsigned long long q = next() >> (next() % 64);
            unsigned long long m = q * d;
            if (m / d == q) {
                if (check(m, d) || check(m - 1, d) || check(m + 1, d)) return 3;
            }
        }
    }
    return 0;
}
