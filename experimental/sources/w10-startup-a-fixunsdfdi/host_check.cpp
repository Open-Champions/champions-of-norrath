// Host comparison only. Not an EE build input. The six soft-double callees are modelled with
// host doubles (in the second pass the first dptoul, which forms the high half, rounds to nearest,
// so the negative-remainder path is exercised); the result is compared with the host's own conversion.
#include <cmath>
#include <cstdio>
#include <cstring>
#include "fixunsdfdi.cpp"
static bool nearest;
static int toulCalls;
static double D(unsigned long long b) { double d; std::memcpy(&d, &b, 8); return d; }
static unsigned long long B(double d) { unsigned long long b; std::memcpy(&b, &d, 8); return b; }
extern "C" int dpcmp(unsigned long long l, unsigned long long r) { return D(l) < D(r) ? -1 : D(l) > D(r) ? 1 : 0; }
extern "C" unsigned long long dpmul(unsigned long long l, unsigned long long r) { return B(D(l) * D(r)); }
extern "C" unsigned dptoul(unsigned long long v) { double d = (nearest && toulCalls++ == 0) ? std::nearbyint(D(v)) : std::trunc(D(v)); return static_cast<unsigned>(static_cast<long long>(d)); }
extern "C" unsigned long long __floatdidf(long long v) { return B(static_cast<double>(v)); }
extern "C" unsigned long long dpadd(unsigned long long l, unsigned long long r) { return B(D(l) + D(r)); }
extern "C" unsigned long long dpsub(unsigned long long l, unsigned long long r) { return B(D(l) - D(r)); }
int main() {
    int failures = 0;
    const double v[] = {0.0, 1.0, 1.5, 4294967295.0, 4294967296.0, 4294967297.0, 123456789012.0, 9223372036854775808.0,
                        9223372036854777856.0, 18446744073709549568.0, 1e19, -1.0, -1e30};
    for (int pass = 0; pass < 2; ++pass) {
        nearest = pass == 1;
        for (double d : v) {
            toulCalls = 0;
            if (nearest && d > 1.8e19 && d < 1.85e19) continue;  // high half would round up to 2^32
            unsigned long long want = d < 0 ? 0ull : static_cast<unsigned long long>(d);
            unsigned long long got = __fixunsdfdi(B(d));
            if (got != want) { std::printf("pass %d %.17g: %llu want %llu\n", pass, d, got, want); ++failures; }
        }
    }
    if (failures) return 2;
    std::printf("fixunsdfdi cases passed\n");
    return 0;
}
