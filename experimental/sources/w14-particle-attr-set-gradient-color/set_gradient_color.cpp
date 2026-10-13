// Observed: SetGradientColor__20VIParticleAttributesiRC10VIColor32F is 56 bytes. It copies
// 16 bytes from the color argument to attrs + 56 + 16 * index with two unaligned 64-bit
// moves (no bounds check on the index), then adds one to the word at offset 716.
// Inferred: the attribute block starts an array of 16-byte gradient colors at offset 56.
// Unknown: the number of entries (the accessor GradientColor__C20VIParticleAttributesi is
// not authored here) and what consumes the counter.
extern "C" {
void SetGradientColor__20VIParticleAttributesiRC10VIColor32F(unsigned char* attrs, int index, const void* color) {
    __builtin_memcpy(attrs + 56 + index * 16, color, 16);
    *reinterpret_cast<unsigned*>(attrs + 716) += 1;
}
}
