#include "set_gradient_color.cpp"
#include <string.h>

int main() {
    alignas(4) unsigned char block[768];
    unsigned char color[17];
    for (int index = 0; index < 4; index++) {
        memset(block, 0x5a, sizeof block);
        for (int i = 0; i < 17; i++) color[i] = (unsigned char)(i + 1 + index);
        *reinterpret_cast<unsigned*>(block + 716) = 100 + index;
        SetGradientColor__20VIParticleAttributesiRC10VIColor32F(block, index, color + 1);
        int base = 56 + 16 * index;
        for (int i = 0; i < 16; i++) if (block[base + i] != color[1 + i]) return 1;
        if (*reinterpret_cast<unsigned*>(block + 716) != 101u + index) return 2;
        for (int i = 0; i < 768; i++) {
            if (i >= base && i < base + 16) continue;
            if (i >= 716 && i < 720) continue;
            if (block[i] != 0x5a) return 3;
        }
    }
    return 0;
}
