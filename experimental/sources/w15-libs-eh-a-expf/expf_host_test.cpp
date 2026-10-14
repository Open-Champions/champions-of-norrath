// Host check only; not an EE build input. Build the source with -Dexpf=w15_expf.
// The callee is a stub that records its argument bits and returns a marker.
#include <cstdio>
#include <cstring>
static unsigned seen_bits; static int calls;
static float from_bits(unsigned b) { float f; std::memcpy(&f, &b, 4); return f; }
static unsigned to_bits(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
extern "C" float __ieee754_expf(float x) { ++calls; seen_bits = to_bits(x); return 7.5f; }
extern "C" float w15_expf(float);
int main() {
    const unsigned patterns[] = {0u, 0x80000000u, 0x3f800000u, 0xbf800000u, 0x7f800000u, 0x7fc00001u, 0x00000001u};
    for (unsigned p : patterns) {
        calls = 0;
        if (w15_expf(from_bits(p)) != 7.5f) return 1;
        if (calls != 1 || seen_bits != p) return 2;
    }
    std::puts("expf ok");
    return 0;
}
