// Observed: texNormalizedHeight__FP8_texture at 0x00172ab0 is 8 bytes in the pinned SLUS-20565 image. It returns the signed
// halfword at offset 2 of its argument, sign extended to an int.
// Inferred: the argument is a texture record whose halfword at +2 is the height, paired with texNormalizedWidth at +0. "Normalized"
// is only the original name; whether the value is scaled is unknown.
// Unknown: the rest of the texture record. Nothing here ran on the EE.
extern "C" {
int texNormalizedHeight__FP8_texture(const unsigned char* texture) {
    return *reinterpret_cast<const short*>(texture + 2);
}
}
