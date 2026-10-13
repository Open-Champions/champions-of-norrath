// Observed: SetBirthrateVar__20VIParticleAttributesf at 0x010b45c8 is 20 bytes. It stores its float argument at
// offset 8 of the attribute block and then adds one to the word at offset 716.
// Inferred: offset 8 holds the birth rate variance and the word at 716 counts modifications.
// Unknown: what consumes the counter, and the block's total size.
extern "C" {
void SetBirthrateVar__20VIParticleAttributesf(unsigned char* attrs, float value) {
    *reinterpret_cast<float*>(attrs + 8) = value;
    *reinterpret_cast<unsigned*>(attrs + 716) += 1;
}
}
