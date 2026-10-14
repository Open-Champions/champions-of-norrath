// Host contract for ParticleEmitterArray. Not an EE plan input.
extern "C" unsigned char ParticleEmitterArray[16 * 152];
int main() {
    if (sizeof ParticleEmitterArray != 2432u) return 1;
    for (unsigned i = 0; i < sizeof ParticleEmitterArray; ++i) {
        if (ParticleEmitterArray[i] != 0) return 2;
    }
    return 0;
}
