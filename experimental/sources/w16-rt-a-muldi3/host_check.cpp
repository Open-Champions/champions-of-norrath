// Host check (not a plan input). Build:
//   c++ -O1 -c -D__muldi3=w16_muldi3 -o /tmp/w16_md_src.o muldi3.cpp
//   c++ -O1 -o /tmp/w16_muldi3 host_check.cpp /tmp/w16_md_src.o && /tmp/w16_muldi3
#include <cstdio>
#include <random>

extern "C" unsigned long long w16_muldi3(unsigned long long left, unsigned long long right);

int main() {
    unsigned cases = 0;
    const unsigned long long edge[] = {0, 1, 2, 0xffffffffull, 0x100000000ull, 0x7fffffffffffffffull,
                                       0x8000000000000000ull, 0xffffffffffffffffull, 0xdeadbeefcafef00dull};
    for (unsigned long long a : edge)
        for (unsigned long long b : edge) {
            if (w16_muldi3(a, b) != a * b) { std::printf("edge %llx %llx\n", a, b); return 1; }
            ++cases;
        }
    std::mt19937_64 rng(11);
    for (int i = 0; i < 200000; ++i) {
        unsigned long long a = rng(), b = rng();
        if (i & 1) a >>= (rng() % 64);
        if (i & 2) b >>= (rng() % 64);
        if (w16_muldi3(a, b) != a * b) { std::printf("random %llx %llx\n", a, b); return 2; }
        ++cases;
    }
    std::printf("%u muldi3 cases passed\n", cases);
    return 0;
}
