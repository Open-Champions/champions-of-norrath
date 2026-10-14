#include <string.h>
float PrimNormalDef[4] __asm__("_12VIPrimBuffer$PrimNormal");
#include "normal.cpp"

int main() {
    float v[3] = {1.5f, -2.25f, 1e-30f};
    PrimNormalDef[3] = 9.0f;
    if (Normal__12VIPrimBufferR7VIVect3(0, v) != 0) return 1;
    if (memcmp(PrimNormalDef, v, 12) != 0) return 2;
    if (PrimNormalDef[3] != 9.0f) return 3;
    return 0;
}
