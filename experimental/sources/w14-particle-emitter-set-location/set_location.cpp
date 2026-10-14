// Observed: SetLocation__17VIParticleEmitterRC7VIVect3 at 0x010b8b78 is 100 bytes. It
// copies the three floats at offset 16 of the emitter to offset 28, then copies the three
// floats of the argument to offset 16. When the word at offset 980 was zero before the
// stores it then copies the new location over the three floats at 28 again (x first, then
// stores 1 into the word at 980, then y and z), so the first call leaves both triples equal
// to the argument. The flag is read before any store.
// Inferred: offset 16 is the current location and 28 the previous location (LastLocation__C
// 17VIParticleEmitter is the neighbouring accessor), and the word at 980 is a "location has
// been set once" flag.
// Unknown: the flag's other uses.
extern "C" {
void SetLocation__17VIParticleEmitterRC7VIVect3(unsigned char* emitter, const float* location) {
    float* current = reinterpret_cast<float*>(emitter + 16);
    float* last = reinterpret_cast<float*>(emitter + 28);
    unsigned initialized = *reinterpret_cast<unsigned*>(emitter + 980);
    last[0] = current[0];
    last[1] = current[1];
    last[2] = current[2];
    current[0] = location[0];
    current[1] = location[1];
    current[2] = location[2];
    if (initialized == 0) {
        last[0] = current[0];
        *reinterpret_cast<unsigned*>(emitter + 980) = 1;
        last[1] = current[1];
        last[2] = current[2];
    }
}
}
