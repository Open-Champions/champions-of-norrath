// Observed: SetGradientRepeat__20VIParticleAttributesf at 0x010b47e0 is 20 bytes. It stores its float argument at
// offset 608 of the attribute block and then adds one to the word at offset 716.
// Inferred: offset 608 holds the gradient repeat and the word at 716 counts modifications.
// Unknown: what consumes the counter, and the block's total size.
extern "C" {
void SetGradientRepeat__20VIParticleAttributesf(unsigned char* attrs, float value) {
    *reinterpret_cast<float*>(attrs + 608) = value;
    *reinterpret_cast<unsigned*>(attrs + 716) += 1;
}
}
