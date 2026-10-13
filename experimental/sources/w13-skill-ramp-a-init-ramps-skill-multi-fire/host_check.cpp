#include <cstdio>
#include <cstring>
extern "C" void init_ramps(void*) asm("initRamps__14SkillMultiFire");
struct Store { unsigned offset; unsigned width; unsigned bits; };
// Expected stores as raw little-endian bit patterns, independent of the source's literals.
static const Store kStores[] = {{14,2,0x1u},{16,2,0x1u},{18,2,0x1u},{22,2,0x1u},{24,2,0x78u},{26,2,0x1deu},{28,2,0x2u},{32,2,0x5u},{36,4,0x41200379u},{40,4,0x40e96fefu},{44,4,0x3e4dd377u},{52,4,0x3fffff3fu},{56,2,0x1u},{58,2,0x1u},{60,2,0x1u},{66,2,0x2u},{68,2,0x1c2u},{70,2,0x2u},{76,4,0x40000000u},{80,4,0x40800000u},{84,4,0x3dccccccu}};
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
