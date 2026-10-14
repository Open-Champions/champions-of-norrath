#include "set_inherit_velocity.cpp"
#include <string.h>

int main() {
    alignas(4) unsigned char block[768];
    memset(block, 0x5a, sizeof block);
    *reinterpret_cast<unsigned*>(block + 716) = 41;
    SetInheritVelocity__20VIParticleAttributesf(block, 2.5f);
    if (*reinterpret_cast<float*>(block + 600) != 2.5f) return 1;
    if (*reinterpret_cast<unsigned*>(block + 716) != 42) return 2;
    for (int i = 0; i < 768; i++) {
        if (i >= 600 && i < 600 + 4) continue;
        if (i >= 716 && i < 720) continue;
        if (block[i] != 0x5a) return 3;
    }
    *reinterpret_cast<unsigned*>(block + 716) = 0xffffffffu;
    SetInheritVelocity__20VIParticleAttributesf(block, -0.0f);
    if (*reinterpret_cast<unsigned*>(block + 600) != 0x80000000u) return 4;
    if (*reinterpret_cast<unsigned*>(block + 716) != 0) return 5;
    return 0;
}
