#include "set_gravity_on.cpp"
#include <string.h>

int main() {
    alignas(4) unsigned char block[768];
    memset(block, 0x5a, sizeof block);
    *reinterpret_cast<unsigned*>(block + 716) = 3;
    SetGravityOn__20VIParticleAttributesi(block, 1);
    if (*reinterpret_cast<unsigned*>(block + 720) != 0xc0a00000u) return 1;
    if (*reinterpret_cast<unsigned*>(block + 716) != 4) return 2;
    SetGravityOn__20VIParticleAttributesi(block, -7);
    if (*reinterpret_cast<unsigned*>(block + 720) != 0xc0a00000u) return 3;
    SetGravityOn__20VIParticleAttributesi(block, 0);
    if (*reinterpret_cast<unsigned*>(block + 720) != 0) return 4;
    if (*reinterpret_cast<unsigned*>(block + 716) != 6) return 5;
    for (int i = 0; i < 768; i++) {
        if (i >= 716 && i < 724) continue;
        if (block[i] != 0x5a) return 6;
    }
    return 0;
}
