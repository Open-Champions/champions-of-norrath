#include <string.h>
float PrimUV0Def[4] __asm__("_12VIPrimBuffer$PrimUV0");
#include "uv0.cpp"

int main() {
    float src[2] = {0.25f, -7.5f};
    PrimUV0Def[0] = PrimUV0Def[1] = PrimUV0Def[2] = PrimUV0Def[3] = 3.0f;
    if (UV0__12VIPrimBufferR7VIVect2(0, src) != 0) return 1;
    if (PrimUV0Def[0] != 0.25f || PrimUV0Def[1] != -7.5f) return 2;
    if (PrimUV0Def[2] != 3.0f || PrimUV0Def[3] != 3.0f) return 3;
    return 0;
}
