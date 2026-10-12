// Host contract for nmParticleEmitterRecs. Not an EE plan input.
extern "C" int nmParticleEmitterRecs;
int main() {
    if (sizeof nmParticleEmitterRecs != 4u) return 1;
    return nmParticleEmitterRecs == 0 ? 0 : 2;
}
