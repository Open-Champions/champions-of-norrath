// Host check: compares the word-only routine with the host's native signed 64-bit division over
// edge values and a deterministic pseudo-random sweep covering every divisor/dividend size class
// and every sign combination. A zero divisor is excluded (the routine deliberately traps there).
#include "divdi3.cpp"
#include <cstdio>

static unsigned long long state = 0x9e3779b97f4a7c15ull;
static unsigned long long next() {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return state;
}

static int check(long long n, long long d) {
    if (d == 0) return 0;
    long long want = (n == (-0x7fffffffffffffffll - 1) && d == -1) ? n : n / d;
    if (__divdi3(n, d) != want) {
        std::printf("mismatch %lld / %lld\n", n, d);
        return 1;
    }
    return 0;
}

int main() {
    const long long edge[] = {1ll, 2ll, 3ll, 0xffll, 0x100ll, 0xffffll, 0x10000ll, 0xffffffll, 0x1000000ll,
                              0x7fffffffll, 0x80000000ll, 0xffffffffll, 0x100000000ll, 0x100000001ll,
                              0x1ffffffffll, 0x7fffffffffffffffll, -0x7fffffffffffffffll - 1,
                              -0x100000000ll, -2ll, -1ll, 0ll};
    for (long long n : edge)
        for (long long d : edge)
            if (check(n, d) || check(-n, d) || check(n, -d)) return 1;
    for (int i = 0; i < 2000000; ++i) {
        long long n = static_cast<long long>(next() >> (next() % 64));
        long long d = static_cast<long long>(next() >> (next() % 64));
        if (next() & 1) n = -n;
        if (next() & 1) d = -d;
        if (check(n, d)) return 2;
        if (d != 0) {
            long long q = static_cast<long long>(next() >> (next() % 64 + 1));
            __int128 wide = static_cast<__int128>(q) * d;
            if (wide > -0x7ffffffffffffff0ll && wide < 0x7ffffffffffffff0ll) {
                long long m = static_cast<long long>(wide);
                if (check(m, d) || check(m - 1, d) || check(m + 1, d)) return 3;
            }
        }
    }
    std::printf("divdi3 cases passed\n");
    return 0;
}
