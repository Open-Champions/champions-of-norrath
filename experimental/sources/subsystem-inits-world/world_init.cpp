// worldInit__Fv at 0x0017dc78, 64 bytes, read from the pinned SLUS-20565 image.
// Fresh compilation is not an original-byte match. No frustum has been applied
// on the EE.
//
// farClip, at gp-4120, is stored with the bits 0x44e10000 before
// worldSetFrustum__Fffff. The four float arguments are the bits 0x42480000,
// the .lit4 word at 0x004800c4 (0x3faa3d70), 0x42a00000, and 0x3f800000.
// Naming 0x44e10000 as 1800 and the others as 50, 80, and 1 follows those bits.
extern "C" {
extern float farClip;
void worldSetFrustum__Fffff(float first, float second, float third, float fourth);

static float norrath_bits_float(unsigned bits) {
    union {
        float value;
        unsigned bits;
    } converted;
    converted.bits = bits;
    return converted.value;
}

void worldInit__Fv() {
    farClip = norrath_bits_float(0x44e10000u);
    worldSetFrustum__Fffff(norrath_bits_float(0x42480000u),
                           norrath_bits_float(0x3faa3d70u),
                           norrath_bits_float(0x42a00000u),
                           norrath_bits_float(0x3f800000u));
}
}
