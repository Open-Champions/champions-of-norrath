// Host contract for isEnemy__6Player; not an EE input. Includes the source.
#include "is_enemy.cpp"
#include <cstring>
#include <initializer_list>
int main() {
    alignas(8) unsigned char other[1024];
    for (unsigned base : {0x20u}) {
        for (unsigned extra = 0; extra < 0x40; ++extra) {
            // bits in the flag word: 0x20 required; 0x8, 0x40, 0x1000000 forbidden
            unsigned flags = base;
            if (extra & 1) flags |= 0x8u;
            if (extra & 2) flags |= 0x40u;
            if (extra & 4) flags |= 0x1000000u;
            if (extra & 8) flags |= 0xf0f0f0f0u & ~0x20u & ~0x8u & ~0x40u & ~0x1000000u;
            short h = (extra & 16) ? 5 : 0;
            unsigned long long wide = 0;
            if (extra & 32) wide |= 0x800ull;
            for (int variant = 0; variant < 3; ++variant) {
                unsigned long long w = wide;
                if (variant == 1) w |= 0x4000000ull;
                if (variant == 2) w |= 0xffffffffffffffffull & ~0x4000000ull & ~0x800ull;
                std::memset(other, 0, sizeof other);
                std::memcpy(other + 12, &flags, 4);
                std::memcpy(other + 792, &h, 2);
                std::memcpy(other + 160, &w, 8);
                bool expect = (flags & 0x20u) && !(flags & 0x8u) && !(flags & 0x40u) && !(flags & 0x1000000u) && h == 0 &&
                              !(w & 0x4000000ull) && !(w & 0x800ull);
                if (isEnemy__6PlayerP10GameObject(nullptr, other) != (expect ? 1 : 0)) return 1;
            }
        }
    }
    // Missing required bit.
    std::memset(other, 0, sizeof other);
    if (isEnemy__6PlayerP10GameObject(nullptr, other) != 0) return 2;
    unsigned f = 0x20;
    std::memcpy(other + 12, &f, 4);
    if (isEnemy__6PlayerP10GameObject(nullptr, other) != 1) return 3;
    return 0;
}
