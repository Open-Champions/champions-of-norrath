#include <string.h>
unsigned char PrimNormalS8Def[8] __asm__("_12VIPrimBuffer$PrimNormalS8");
#include "normal.cpp"

int main() {
    unsigned char src[6] = {0x81, 0x7f, 0x22, 0xee, 0xdd, 0xcc};
    memset(PrimNormalS8Def, 0x5a, sizeof PrimNormalS8Def);
    if (Normal__12VIPrimBufferR9VIVect3S8(0, src) != 0) return 1;
    if (memcmp(PrimNormalS8Def, src, 3) != 0) return 2;
    for (int i = 3; i < 8; ++i)
        if (PrimNormalS8Def[i] != 0x5a) return 3;
    return 0;
}
