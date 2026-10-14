// Observed: Space__C17VIParticleEmitter at 0x010b8b40, 8 bytes. It returns its argument plus 96 and reads nothing.
// Inferred: an embedded member at byte 96 (accessor returns its address). Unknown: the member name and the embedded type.
// Nothing here ran on the EE; the paired comparison against the original used a private interpreter.
extern "C" {
const void* Space__C17VIParticleEmitter(const void* object) {
    return static_cast<const unsigned char*>(object) + 96;
}
}
