#include "set_start_color_var.cpp"
#include <string.h>

int main() {
    alignas(4) unsigned char block[768];
    unsigned char color[17];
    memset(block, 0x5a, sizeof block);
    for (int i = 0; i < 17; i++) color[i] = (unsigned char)(i + 1);
    *reinterpret_cast<unsigned*>(block + 716) = 7;
    SetStartColorVar__20VIParticleAttributesRC10VIColor32F(block, color + 1);
    for (int i = 0; i < 16; i++) if (block[568 + i] != color[1 + i]) return 1;
    if (*reinterpret_cast<unsigned*>(block + 716) != 8) return 2;
    for (int i = 0; i < 768; i++) {
        if (i >= 568 && i < 568 + 16) continue;
        if (i >= 716 && i < 720) continue;
        if (block[i] != 0x5a) return 3;
    }
    return 0;
}
