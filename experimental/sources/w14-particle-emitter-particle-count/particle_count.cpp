// Observed: ParticleCount__C17VIParticleEmitter at 0x010b8cf8 is 8 bytes. It returns the
// word at offset 172 of the emitter. Destroy__17VIParticleEmitter stores zero there.
// Inferred: the word is the number of live particles; the name comes from the symbol.
// Unknown: which routine increments it.
extern "C" {
int ParticleCount__C17VIParticleEmitter(const unsigned char* emitter) {
    return *reinterpret_cast<const int*>(emitter + 172);
}
}
