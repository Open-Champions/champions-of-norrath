#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" const unsigned char norrath_factory_ctype_minimum_view[];

static std::uint32_t fnv(const unsigned char* data, std::size_t size) {
    std::uint32_t hash = 2166136261u;
    for (std::size_t i = 0; i < size; ++i) {
        hash ^= data[i];
        hash *= 16777619u;
    }
    return hash;
}

int main() {
    const unsigned char* view = norrath_factory_ctype_minimum_view;
    if ((view['A' + 128] & 1u) == 0 || view['A' + 128] != 0x41) return 1;
    if ((view['Z' + 128] & 1u) == 0) return 2;
    if ((view['a' + 128] & 1u) != 0 || view['a' + 128] != 0x42) return 3;
    if ((view['z' + 128] & 1u) != 0) return 4;
    if (view['0' + 128] != 0x04) return 5;
    if (view[' ' + 128] != 0x88) return 6;
    if (view[127] != 0) return 7;
    if (view[255 + 128] != 0) return 8;
    // FNV-1a of the 384 observed bytes. A shifted or truncated table fails.
    if (fnv(view, 384) != 0xc7e96547u) return 9;
    std::printf("ctype view cases passed\n");
    return 0;
}
