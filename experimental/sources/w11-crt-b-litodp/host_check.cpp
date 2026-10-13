// Host comparison only. Not an EE build input. The pack step is replaced by a host stand-in that
// builds a double from the record fields; the result must equal the host int-to-double conversion.
#include <cstdio>
#include <cstring>
#include <cstdint>
#include "litodp.cpp"
extern "C" unsigned long long __pack_d(const W11CrtBUnpackedDouble* v) {
    if (v->cls == 2) return 0;
    unsigned long long e = static_cast<unsigned long long>(v->power + 1023);
    return (static_cast<unsigned long long>(v->sign) << 63) | (e << 52) | ((v->fraction >> 8) & ((1ull << 52) - 1));
}
int main() {
    int failures = 0;
    const int cases[] = {0, 1, -1, 2, 3, 255, -256, 65535, 0x12345678, -0x12345678, 0x7fffffff, -0x7fffffff, INT32_MIN, 1 << 30, -(1 << 30)};
    for (int v : cases) {
        double d = static_cast<double>(v);
        unsigned long long want;
        std::memcpy(&want, &d, 8);
        unsigned long long got = litodp(v);
        if (got != want) { std::printf("%d: got %016llx want %016llx\n", v, got, want); ++failures; }
    }
    for (int bit = 0; bit < 31; ++bit) {
        for (int delta = -1; delta <= 1; ++delta) {
            int v = (1 << bit) + delta;
            for (int s = 1; s >= -1; s -= 2) {
                int x = v * s;
                double d = x;
                unsigned long long want;
                std::memcpy(&want, &d, 8);
                if (litodp(x) != want) { std::printf("%d wrong\n", x); ++failures; }
            }
        }
    }
    if (failures) return 2;
    std::printf("litodp cases passed\n");
    return 0;
}
