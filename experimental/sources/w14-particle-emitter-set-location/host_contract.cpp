#include "set_location.cpp"
#include <string.h>

int main() {
    alignas(4) unsigned char e[1024];
    memset(e, 0x5a, sizeof e);
    float* cur = reinterpret_cast<float*>(e + 16);
    float* last = reinterpret_cast<float*>(e + 28);
    cur[0] = 1; cur[1] = 2; cur[2] = 3;
    *reinterpret_cast<unsigned*>(e + 980) = 0;
    float a[3] = {10, 11, 12};
    SetLocation__17VIParticleEmitterRC7VIVect3(e, a);
    if (cur[0] != 10 || cur[1] != 11 || cur[2] != 12) return 1;
    if (last[0] != 10 || last[1] != 11 || last[2] != 12) return 2;
    if (*reinterpret_cast<unsigned*>(e + 980) != 1) return 3;
    float b[3] = {20, 21, 22};
    SetLocation__17VIParticleEmitterRC7VIVect3(e, b);
    if (cur[0] != 20 || cur[1] != 21 || cur[2] != 22) return 4;
    if (last[0] != 10 || last[1] != 11 || last[2] != 12) return 5;
    *reinterpret_cast<unsigned*>(e + 980) = 0xffffffffu;
    SetLocation__17VIParticleEmitterRC7VIVect3(e, a);
    if (last[0] != 20 || last[2] != 22) return 6;
    for (int i = 0; i < 1024; i++) {
        if (i >= 16 && i < 40) continue;
        if (i >= 980 && i < 984) continue;
        if (e[i] != 0x5a) return 7;
    }
    return 0;
}
