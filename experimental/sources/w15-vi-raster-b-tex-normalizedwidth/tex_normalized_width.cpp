// Observed: texNormalizedWidth__FP8_texture at 0x00172aa8 is 8 bytes in the pinned SLUS-20565 image. It returns the signed
// halfword at offset 0 of its argument, sign extended to an int.
// Inferred: the argument is a texture record whose halfword at +0 is the width, paired with texNormalizedHeight at +2. "Normalized"
// is only the original name; whether the value is scaled is unknown.
// Unknown: the rest of the texture record. Nothing here ran on the EE.
extern "C" {
int texNormalizedWidth__FP8_texture(const unsigned char* texture) {
    return *reinterpret_cast<const short*>(texture);
}
}
