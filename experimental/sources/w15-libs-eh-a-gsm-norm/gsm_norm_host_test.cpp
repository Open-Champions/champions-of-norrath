// Host check only; not an EE build input. Build the source with -Dgsm_norm=w15_gsm_norm; this file is compiled without
// it and defines bitoff (8 minus the bit length of the index). The reference counts left shifts until bit 30 of the
// (complemented, if negative) 32-bit value is set, with the same special cases as the comment in the source.
#include <cstdio>
extern "C" {
unsigned char bitoff[256];
int w15_gsm_norm(long long value);
}
static int reference(long long a) {
    if (a < 0) {
        if (a <= -1073741824) return 0;
        a = ~a;
    }
    if (a == 0) return 31;
    int shifts = 0;
    while (!(a & 0x40000000)) { a <<= 1; ++shifts; }
    return shifts;
}
int main() {
    for (int i = 0; i < 256; ++i) { int bits = 0; for (int v = i; v; v >>= 1) ++bits; bitoff[i] = static_cast<unsigned char>(8 - bits); }
    unsigned long long s = 88172645463325252ull;
    for (int round = 0; round < 400000; ++round) {
        s ^= s << 13; s ^= s >> 7; s ^= s << 17;
        const int width = 1 + static_cast<int>(s % 32);
        long long v = static_cast<long long>(s >> 8 & ((1ull << width) - 1));
        if (round % 2) v = -v;
        if (v > 2147483647ll || v < -2147483647ll) continue;
        if (w15_gsm_norm(v) != reference(v)) { std::printf("mismatch %lld\n", v); return 1; }
    }
    for (int k = 0; k < 31; ++k) {
        const long long edges[] = {1ll << k, (1ll << k) - 1, -(1ll << k), -(1ll << k) - 1, -(1ll << k) + 1};
        for (long long v : edges) if (w15_gsm_norm(v) != reference(v)) return 2;
    }
    std::puts("gsm_norm ok");
    return 0;
}
