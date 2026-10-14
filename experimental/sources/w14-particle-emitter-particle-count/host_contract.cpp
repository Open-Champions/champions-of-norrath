#include "particle_count.cpp"

int main() {
    alignas(4) unsigned char emitter[256] = {};
    *reinterpret_cast<int*>(emitter + 172) = 37;
    if (ParticleCount__C17VIParticleEmitter(emitter) != 37) return 1;
    *reinterpret_cast<int*>(emitter + 172) = -5;
    if (ParticleCount__C17VIParticleEmitter(emitter) != -5) return 2;
    return 0;
}
