#include <string.h>
unsigned short PrimUV0S16Def[4] __asm__("_12VIPrimBuffer$PrimUV0S16");
#include "uv0.cpp"

int main() {
    unsigned short src[2] = {0x1234, 0xfffe};
    PrimUV0S16Def[0] = PrimUV0S16Def[1] = PrimUV0S16Def[2] = PrimUV0S16Def[3] = 0x5a5a;
    if (UV0__12VIPrimBufferR10VIVect2S16(0, src) != 0) return 1;
    if (PrimUV0S16Def[0] != 0x1234 || PrimUV0S16Def[1] != 0xfffe) return 2;
    if (PrimUV0S16Def[2] != 0x5a5a || PrimUV0S16Def[3] != 0x5a5a) return 3;
    return 0;
}
