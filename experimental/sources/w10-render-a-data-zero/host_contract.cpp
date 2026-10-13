// Host contract for the zero storage objects. Not an EE plan input.
#include <cstdio>
#include "data_zero.cpp"
int main() {
    for (unsigned i = 0; i != sizeof ParticleEmitterTestParticle; ++i) if (ParticleEmitterTestParticle[i] || droplet[i]) return 1;
    for (unsigned i = 0; i != sizeof fontGrid; ++i) if (fontGrid[i]) return 2;
    for (int r = 0; r != 4; ++r) for (int c = 0; c != 4; ++c) if (castLightColor[r][c]) return 3;
    if (displayTextDisable != 0 || animLoadBuffer != 0 || bevel != nullptr) return 4;
    std::puts("data-zero contract passed");
    return 0;
}
