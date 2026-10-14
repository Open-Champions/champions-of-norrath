#include <cstdio>
#include <cstring>
extern "C" void init_ramps(void*) asm("initRamps__14SkillColdArrow");
struct Store { unsigned offset; unsigned width; unsigned bits; };
// Expected stores as raw little-endian bit patterns, independent of the source's literals.
static const Store kStores[] = {{14,2,0x1u},{16,2,0x1u},{18,2,0x1u},{20,2,0x1u},{22,2,0x1u},{24,2,0x28u},{26,2,0x1f2u},{28,2,0x6u},{30,2,0x3u},{32,2,0xeu},{36,4,0x40a006e5u},{40,4,0x40b3582fu},{44,4,0x409fffd3u},{48,4,0x3ea9fc4cu},{52,4,0x403fff82u},{56,2,0x1u},{66,2,0x2u},{76,4,0x40000000u}};
int main() {
    alignas(8) unsigned char bytes[160];
    unsigned char want[160];
    std::memset(bytes, 0xa5, sizeof bytes);
    std::memset(want, 0xa5, sizeof want);
    for (const Store& s : kStores)
        for (unsigned i = 0; i < s.width; ++i) want[s.offset + i] = (unsigned char)(s.bits >> (8 * i));
    init_ramps(bytes);
    for (unsigned i = 0; i < sizeof bytes; ++i)
        if (bytes[i] != want[i]) { std::fprintf(stderr, "offset %u: got %u want %u\n", i, bytes[i], want[i]); return 1; }
    return 0;
}
