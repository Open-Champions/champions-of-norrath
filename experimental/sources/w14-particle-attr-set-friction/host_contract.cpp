#include "set_friction.cpp"
#include <string.h>

int main() {
    alignas(4) unsigned char block[768];
    memset(block, 0x5a, sizeof block);
    *reinterpret_cast<unsigned*>(block + 716) = 41;
    SetFriction__20VIParticleAttributesf(block, 2.5f);
    if (*reinterpret_cast<float*>(block + 0) != 2.5f) return 1;
    if (*reinterpret_cast<unsigned*>(block + 716) != 42) return 2;
    for (int i = 0; i < 768; i++) {
        if (i >= 0 && i < 0 + 4) continue;
        if (i >= 716 && i < 720) continue;
        if (block[i] != 0x5a) return 3;
    }
    *reinterpret_cast<unsigned*>(block + 716) = 0xffffffffu;
    SetFriction__20VIParticleAttributesf(block, -0.0f);
    if (*reinterpret_cast<unsigned*>(block + 0) != 0x80000000u) return 4;
    if (*reinterpret_cast<unsigned*>(block + 716) != 0) return 5;
    return 0;
}
