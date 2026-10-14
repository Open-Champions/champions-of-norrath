// Observed: Direction__C17VIParticleEmitter at 0x010b8be0, 8 bytes. It returns its argument plus 40 and reads nothing.
// Inferred: an embedded member at byte 40 (accessor returns its address). Unknown: the member name and the embedded type.
// Nothing here ran on the EE; the paired comparison against the original used a private interpreter.
extern "C" {
const void* Direction__C17VIParticleEmitter(const void* object) {
    return static_cast<const unsigned char*>(object) + 40;
}
}
