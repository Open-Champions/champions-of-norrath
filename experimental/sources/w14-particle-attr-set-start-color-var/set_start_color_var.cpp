// Observed: SetStartColorVar__20VIParticleAttributesRC10VIColor32F is 48 bytes. It copies 16 bytes
// from the color argument to offset 568 of the attribute block with two unaligned 64-bit
// moves, then adds one to the word at offset 716.
// Inferred: the 16 bytes are four floats (VIColor32F) holding the start color variance, and the
// word at 716 counts modifications.
// Unknown: what consumes the counter.
extern "C" {
void SetStartColorVar__20VIParticleAttributesRC10VIColor32F(unsigned char* attrs, const void* color) {
    __builtin_memcpy(attrs + 568, color, 16);
    *reinterpret_cast<unsigned*>(attrs + 716) += 1;
}
}
