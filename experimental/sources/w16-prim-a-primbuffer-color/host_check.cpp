#include <string.h>
unsigned char PrimColorDef[8] __asm__("_12VIPrimBuffer$PrimColor");
#include "color.cpp"

int main() {
    unsigned char src[8] = {0xaa, 0x11, 0x22, 0x33, 0x44, 0xbb, 0xcc, 0xdd};
    memset(PrimColorDef, 0x5a, sizeof PrimColorDef);
    for (int off = 1; off < 4; ++off) {
        if (Color__12VIPrimBufferR9VIColor32(0, reinterpret_cast<const PackedColor32*>(src + off)) != 0) return 1;
        if (memcmp(PrimColorDef, src + off, 4) != 0) return 2;
        if (PrimColorDef[4] != 0x5a) return 3;
    }
    return 0;
}
