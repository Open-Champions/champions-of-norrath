/* Host contract. Not a plan input. Compares the routine with an independent
 * formulation: for 0.5 <= |x| < 2^52 the result is floor(4|x|) with a carry when its
 * two low bits are both set; larger values are the low 32 bits of the shifted integer. */
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cmath>
#include "ftoi.cpp"
static unsigned long long bits_of(double d) { unsigned long long u; std::memcpy(&u, &d, 8); return u; }
int main() {
    int bad = 0;
    uint64_t state = 88172645463325252ull;
    auto next = [&]() { state ^= state << 13; state ^= state >> 7; state ^= state << 17; return state; };
    for (int i = 0; i < 200000; ++i) {
        const int e2 = static_cast<int>(next() % 70) - 8;               // magnitudes 2^-8 .. 2^61
        double x = std::ldexp(1.0 + (next() >> 12) / 4503599627370496.0, e2);
        const bool neg = (next() & 1) != 0;
        unsigned long long bits = bits_of(neg ? -x : x);
        int want;
        if (x < 0.5) want = 0;
        else if (x >= std::ldexp(1.0, 65)) want = 9999;
        else if (x < std::ldexp(1.0, 51)) {
            const uint64_t t = static_cast<uint64_t>(x * 4.0);
            want = static_cast<int>(static_cast<uint32_t>(((t & 3) == 3) ? (t >> 2) + 1 : (t >> 2)));
        } else if (x < std::ldexp(1.0, 52)) want = 0;                      // shift of 63
        else {
            const unsigned k = static_cast<unsigned>(std::ilogb(x)) - 52;
            const uint64_t m = ((bits & 0xfffffffffffffull) | (1ull << 52)) << k;
            want = static_cast<int>(static_cast<uint32_t>(m));
        }
        if (ftoi(bits) != want) { if (++bad < 5) std::printf("ftoi wrong for %a: %d want %d\n", neg ? -x : x, ftoi(bits), want); }
    }
    // Fixed edges.
    if (ftoi(0) != 0 || ftoi(bits_of(0.49999)) != 0 || ftoi(bits_of(0.5)) != 0 || ftoi(bits_of(0.75)) != 1 ||
        ftoi(bits_of(0.74999)) != 0 || ftoi(bits_of(1.0)) != 1 || ftoi(bits_of(1.74)) != 1 || ftoi(bits_of(1.75)) != 2 ||
        ftoi(bits_of(-1.75)) != 2 || ftoi(bits_of(1e30)) != 9999 || ftoi(0x7ff0000000000000ull) != 9999 ||
        ftoi(bits_of(1000000.0)) != 1000000 || ftoi(bits_of(65536.0 * 65536.0 * 4)) != 0) { std::puts("edges wrong"); ++bad; }
    if (bad) return 1;
    std::puts("ftoi contract passed");
    return 0;
}
