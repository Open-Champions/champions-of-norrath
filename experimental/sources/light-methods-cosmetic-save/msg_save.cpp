// Observed msg_save__12CosmeticPropPc at 0x155448, 160 bytes.
// Writes 20 bytes and returns 20. The first three words are the results of
// objectSaveAddToStringPool on the pointers at object+76, +80, and +84.
// The next word is the raw word at object+12. The next halfword is the
// cvt.w.s result of the float at object+32, and the last halfword is the
// cvt.w.s result of the float at object+28. cvt.w.s uses round-to-nearest,
// ties to even, which is the MIPS reset mode. This function does not change
// the rounding mode. A result outside a signed halfword keeps its low 16 bits.

extern "C" unsigned objectSaveAddToStringPool__FPCc(const char* text);

namespace {
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
void write_half(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
}
void* pointer_from_word(unsigned value) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(value));
}
float float_from_word(unsigned value) {
    union {
        unsigned bits;
        float number;
    } converted;
    converted.bits = value;
    return converted.number;
}
__attribute__((always_inline)) inline int cvt_w_s(float value) {
    union {
        float as_float;
        unsigned as_bits;
    } bits;
    bits.as_float = value;
    unsigned raw = bits.as_bits;
    int sign = static_cast<int>((raw >> 31) & 1u);
    unsigned exponent_field = (raw >> 23) & 0xffu;
    unsigned fraction = (raw & 0x7fffffu) | (exponent_field == 0u ? 0u : 0x800000u);
    if (exponent_field == 255u) return sign != 0 ? static_cast<int>(0x80000000u) : 0x7fffffff;
    if (exponent_field == 0u) return 0;
    int exponent = static_cast<int>(exponent_field) - 127;
    if (exponent >= 31) return sign != 0 ? static_cast<int>(0x80000000u) : 0x7fffffff;
    if (exponent < 0) {
        if (exponent == -1 && fraction == 0x800000u) return 0;
        if (exponent < -1) return 0;
        return sign != 0 ? -1 : 1;
    }
    int shift = exponent - 23;
    unsigned magnitude;
    if (shift >= 0) magnitude = fraction << shift;
    else {
        int places = -shift;
        unsigned base = fraction >> places;
        unsigned remainder = fraction & ((1u << places) - 1u);
        unsigned half = 1u << (places - 1);
        if (remainder > half || (remainder == half && (base & 1u) != 0u)) base += 1u;
        magnitude = base;
    }
    return sign != 0 ? -static_cast<int>(magnitude) : static_cast<int>(magnitude);
}
}

extern "C" int norrath_vt_msg_save_12CosmeticProp(void* object_storage, unsigned char* destination) {
    const unsigned char* object = static_cast<const unsigned char*>(object_storage);
    const unsigned first = objectSaveAddToStringPool__FPCc(
        static_cast<const char*>(pointer_from_word(read_word(object + 76u))));
    const unsigned second = objectSaveAddToStringPool__FPCc(
        static_cast<const char*>(pointer_from_word(read_word(object + 80u))));
    const unsigned third = objectSaveAddToStringPool__FPCc(
        static_cast<const char*>(pointer_from_word(read_word(object + 84u))));
    write_word(destination, first);
    write_word(destination + 4, second);
    write_word(destination + 8, third);
    write_word(destination + 12, read_word(object + 12u));
    write_half(destination + 16, static_cast<unsigned>(cvt_w_s(float_from_word(read_word(object + 32u)))));
    write_half(destination + 18, static_cast<unsigned>(cvt_w_s(float_from_word(read_word(object + 28u)))));
    return 20;
}
