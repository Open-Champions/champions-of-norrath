// Observed: SetVelocityVar__20VIParticleAttributesf at 0x010b4648 is 20 bytes. It stores its float argument at
// offset 24 of the attribute block and then adds one to the word at offset 716.
// Inferred: offset 24 holds the velocity variance and the word at 716 counts modifications.
// Unknown: what consumes the counter, and the block's total size.
extern "C" {
void SetVelocityVar__20VIParticleAttributesf(unsigned char* attrs, float value) {
    *reinterpret_cast<float*>(attrs + 24) = value;
    *reinterpret_cast<unsigned*>(attrs + 716) += 1;
}
}
