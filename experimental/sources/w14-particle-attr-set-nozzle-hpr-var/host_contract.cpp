#include "set_nozzle_hpr_var.cpp"
#include <string.h>

static int run(float x, float y, float z, unsigned expect_flag) {
    alignas(4) unsigned char block[768];
    memset(block, 0x5a, sizeof block);
    *reinterpret_cast<unsigned*>(block + 716) = 20;
    float v[3] = {x, y, z};
    SetNozzleHprVar__20VIParticleAttributesRC7VIVect3(block, v);
    if (memcmp(block + 672, v, 12) != 0) return 1;
    if (*reinterpret_cast<unsigned*>(block + 700) != expect_flag) return 2;
    if (*reinterpret_cast<unsigned*>(block + 716) != 21) return 3;
    for (int i = 0; i < 768; i++) {
        if (i >= 672 && i < 684) continue;
        if (i >= 700 && i < 704) continue;
        if (i >= 716 && i < 720) continue;
        if (block[i] != 0x5a) return 4;
    }
    return 0;
}

int main() {
    int r;
    if ((r = run(0, 0, 0, 0))) return r;
    if ((r = run(-0.0f, 0, 0, 0))) return 10 + r;
    if ((r = run(1, 0, 0, 1))) return 20 + r;
    if ((r = run(0, 2, 0, 1))) return 30 + r;
    if ((r = run(0, 0, -3, 1))) return 40 + r;
    if ((r = run(0, 0, __builtin_nanf(""), 1))) return 50 + r;
    return 0;
}
