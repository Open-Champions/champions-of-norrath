// Observed: SetInheritVelocity__20VIParticleAttributesf at 0x010b46e8 is 20 bytes. It stores its float argument at
// offset 600 of the attribute block and then adds one to the word at offset 716.
// Inferred: offset 600 holds the inherit-velocity factor and the word at 716 counts modifications.
// Unknown: what consumes the counter, and the block's total size.
extern "C" {
void SetInheritVelocity__20VIParticleAttributesf(unsigned char* attrs, float value) {
    *reinterpret_cast<float*>(attrs + 600) = value;
    *reinterpret_cast<unsigned*>(attrs + 716) += 1;
}
}
