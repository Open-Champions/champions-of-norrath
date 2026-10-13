#include "set_nozzle_axis.cpp"
#include <string.h>

static unsigned char* seen;
static int calls;
extern "C" void Normalize__7VIVect3(void* v) {
    seen = static_cast<unsigned char*>(v);
    calls++;
    // the stored copy must already be in place when Normalize runs
    float* f = static_cast<float*>(v);
    if (f[0] != 1.5f || f[1] != -2.0f || f[2] != 3.25f) calls += 100;
}

int main() {
    alignas(4) unsigned char block[768];
    memset(block, 0x5a, sizeof block);
    float axis[3] = {1.5f, -2.0f, 3.25f};
    *reinterpret_cast<unsigned*>(block + 716) = 9;
    SetNozzleAxis__20VIParticleAttributesRC7VIVect3(block, axis);
    if (calls != 1 || seen != block + 660) return 1;
    if (*reinterpret_cast<unsigned*>(block + 716) != 10) return 2;
    for (int i = 0; i < 768; i++) {
        if (i >= 660 && i < 672) continue;
        if (i >= 716 && i < 720) continue;
        if (block[i] != 0x5a) return 3;
    }
    return 0;
}
