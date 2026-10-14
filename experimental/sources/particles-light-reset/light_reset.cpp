// lightResetOptionalParams__Fv at 0x0013cb88, 316 bytes, read from the pinned
// SLUS-20565 image. 32 direct calls in main .text. Fresh compilation is not
// an original-byte match. No light has been reset on the EE.
//
// scene at 0x004f4060 supplies the bytes. charAmbient, charDirect, and
// charDirect2 are eight-byte halfword groups. Bytes 36 through 43 are stored
// zero-extended. Bytes 48 through 50 are multiplied by the .lit4 word
// 0x3f4ccccc at 0x0047fcb8 and converted with cvt.w.s before the halfword
// store. charScaleR, charScaleG, and charScaleB receive 1.0. interfaceMode
// is compared with slti against 10; a value that is not below 10 is reduced
// by 10. Twelve bytes at scene+24 move to charDirectDir2 and twelve bytes at
// scene+12 move to charDirectDir. ambientOnly and disableVolumetricLighting
// are then stored as zero.
extern "C" {
extern unsigned char scene[];
extern unsigned char charAmbient[];
extern unsigned char charDirect[];
extern unsigned char charDirect2[];
extern unsigned char charDirectDir[];
extern unsigned char charDirectDir2[];
extern float charScaleR;
extern float charScaleG;
extern float charScaleB;
extern int interfaceMode;
extern unsigned ambientOnly;
extern unsigned disableVolumetricLighting;

static unsigned short low_half(int value) {
    return static_cast<unsigned short>(static_cast<unsigned>(value));
}

static void store_half(unsigned char* slot, unsigned short value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
}

static unsigned float_bits(float value) {
    const auto* bytes = reinterpret_cast<const unsigned char*>(&value);
    return static_cast<unsigned>(bytes[0]) | (static_cast<unsigned>(bytes[1]) << 8) |
           (static_cast<unsigned>(bytes[2]) << 16) | (static_cast<unsigned>(bytes[3]) << 24);
}

static float bits_float(unsigned bits) {
    unsigned char bytes[4] = {
        static_cast<unsigned char>(bits), static_cast<unsigned char>(bits >> 8),
        static_cast<unsigned char>(bits >> 16), static_cast<unsigned char>(bits >> 24)};
    float value = 0.f;
    auto* raw = reinterpret_cast<unsigned char*>(&value);
    raw[0] = bytes[0];
    raw[1] = bytes[1];
    raw[2] = bytes[2];
    raw[3] = bytes[3];
    return value;
}

static int round_to_int(float value) {
#if defined(NORRATH_PARTICLES_HOST)
    const unsigned bits = float_bits(value);
    const unsigned raw_exp = (bits >> 23) & 0xffu;
    const unsigned sign = bits >> 31;
    if (raw_exp == 0u || raw_exp == 255u) return static_cast<int>(value);
    const int exponent = static_cast<int>(raw_exp) - 127;
    const unsigned magnitude = (bits & 0x7fffffu) | 0x800000u;
    if (exponent >= 31) return static_cast<int>(value);
    if (exponent < 0) {
        if (exponent < -1 || magnitude == 0x800000u) return 0;
        return sign != 0u ? -1 : 1;
    }
    int whole = 0;
    if (exponent >= 23) {
        whole = static_cast<int>(magnitude << (exponent - 23));
    } else {
        const unsigned shift = static_cast<unsigned>(23 - exponent);
        unsigned truncated = magnitude >> shift;
        const unsigned remainder = magnitude & ((1u << shift) - 1u);
        const unsigned half = 1u << (shift - 1u);
        if (remainder > half || (remainder == half && (truncated & 1u) != 0u)) ++truncated;
        whole = static_cast<int>(truncated);
    }
    return sign != 0u ? -whole : whole;
#else
    return static_cast<int>(value);
#endif
}

void lightResetOptionalParams__Fv() {
    const float scale = bits_float(0x3f4cccccu);
    const int scaled_x = round_to_int(static_cast<float>(scene[48]) * scale);
    const int scaled_y = round_to_int(static_cast<float>(scene[49]) * scale);
    const int scaled_z = round_to_int(static_cast<float>(scene[50]) * scale);
    const int mode = interfaceMode;
    store_half(charAmbient + 6, scene[43]);
    store_half(charDirect2 + 6, scene[39]);
    store_half(charAmbient + 0, scene[40]);
    store_half(charAmbient + 2, scene[41]);
    store_half(charAmbient + 4, scene[42]);
    store_half(charDirect + 0, scene[36]);
    store_half(charDirect + 2, scene[37]);
    store_half(charDirect + 4, scene[38]);
    store_half(charDirect + 6, scene[39]);
    store_half(charDirect2 + 0, low_half(scaled_x));
    store_half(charDirect2 + 2, low_half(scaled_y));
    store_half(charDirect2 + 4, low_half(scaled_z));
    const float one = bits_float(0x3f800000u);
    charScaleR = one;
    charScaleB = one;
    charScaleG = one;
    if (mode >= 10) interfaceMode = mode - 10;
    for (int byte = 0; byte < 12; ++byte) charDirectDir2[byte] = scene[24 + byte];
    for (int byte = 0; byte < 12; ++byte) charDirectDir[byte] = scene[12 + byte];
    ambientOnly = 0u;
    disableVolumetricLighting = 0u;
}
}
