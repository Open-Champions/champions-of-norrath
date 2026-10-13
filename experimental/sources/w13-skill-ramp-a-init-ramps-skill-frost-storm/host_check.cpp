#include <cstdio>
#include <cstring>
extern "C" void init_ramps(void*) asm("initRamps__15SkillFrostStorm");
struct Store { unsigned offset; unsigned width; unsigned bits; };
// Expected stores as raw little-endian bit patterns, independent of the source's literals.
static const Store kStores[] = {{14,2,0x1u},{16,2,0x1u},{18,2,0x1u},{20,2,0x1u},{22,2,0x1u},{24,2,0x14u},{26,2,0x2u},{28,2,0x2u},{30,2,0x1fu},{32,2,0x28u},{36,4,0x407fffb8u},{40,4,0x3eac8909u},{44,4,0x3eaa7f1bu},{48,4,0x400301d1u},{52,4,0x409ffe04u},{56,2,0x1u},{58,2,0x1u},{60,2,0x1u},{62,2,0x1u},{66,2,0x2u},{68,2,0x2u},{70,2,0x3u},{72,2,0x1fu},{76,4,0x40000000u},{80,4,0x3dccccccu},{84,4,0x3d4cccccu},{88,4,0x3f800000u}};
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
