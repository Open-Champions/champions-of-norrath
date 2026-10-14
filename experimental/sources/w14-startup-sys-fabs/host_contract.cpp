// Host contract for fabs. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// Compares the bit-pattern routine with the host fabs on ordinary values, zeros, infinities and NaN payloads,
// and checks that only bit 63 changes.
#include <cmath>
#include <cstdio>
#include <cstring>
#define fabs fabs_hidden
#include "fabs.cpp"
#undef fabs
int main() {
    int bad = 0;
    const unsigned long long pats[] = {0ull, 0x8000000000000000ull, 0x3ff0000000000000ull, 0xbff0000000000000ull,
        0x7ff0000000000000ull, 0xfff0000000000000ull, 0x7ff8123456789abcull, 0xfff4deadbeef0001ull,
        0x0000000000000001ull, 0x8000000000000001ull, 0xffffffffffffffffull, 0x00000000ffffffffull, 0xffffffff00000000ull};
    for (unsigned long long p : pats) {
        unsigned long long r = ::fabs_hidden(p);
        if (r != (p & ~(1ull << 63))) { std::printf("%016llx -> %016llx\n", p, r); ++bad; }
        double d; std::memcpy(&d, &p, 8);
        if (d == d) { double e = std::fabs(d); unsigned long long q; std::memcpy(&q, &e, 8); if (q != r) { std::printf("host %016llx\n", p); ++bad; } }
    }
    return bad;
}
