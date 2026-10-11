// Observed msg_load__12CosmeticPropPc at 0x14b328, 216 bytes.
// Reads 20 bytes and returns 20. The first three words go to
// objectSaveGetFromStringPool and the returned pointers are stored at
// object+76, +80, and +84. The fourth word is stored at object+12.
// The halfword at buffer+16 becomes the float at object+32, and its
// cvt.w.s low byte is stored at object+41. The halfword at buffer+18
// becomes the float at object+28, and its cvt.w.s low byte is stored at
// object+40. cvt.s.w then cvt.w.s uses the MIPS reset rounding mode.
// lumpFindResource then receives the pointers at +76 and +80, and that
// result is stored at +88. A second call receives +76 and +84, and that
// result is stored at +92.

extern "C" char* objectSaveGetFromStringPool__Fi(int index);
extern "C" char* lumpFindResource__FPcT0(char* lump, char* name);

namespace {
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
int read_half(const unsigned char* p) {
    unsigned value = unsigned(p[0]) | (unsigned(p[1]) << 8);
    return value < 0x8000u ? static_cast<int>(value) : static_cast<int>(value) - 65536;
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
unsigned low_pointer(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
void* pointer_from_word(unsigned value) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(value));
}
unsigned float_bits(float value) {
    union {
        float number;
        unsigned bits;
    } converted;
    converted.number = value;
    return converted.bits;
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

extern "C" int norrath_vt_msg_load_12CosmeticProp(void* object_storage, const unsigned char* source) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    char* first = objectSaveGetFromStringPool__Fi(static_cast<int>(read_word(source)));
    char* second = objectSaveGetFromStringPool__Fi(static_cast<int>(read_word(source + 4u)));
    char* third = objectSaveGetFromStringPool__Fi(static_cast<int>(read_word(source + 8u)));
    write_word(object + 76u, low_pointer(first));
    write_word(object + 80u, low_pointer(second));
    write_word(object + 84u, low_pointer(third));
    write_word(object + 12u, read_word(source + 12u));
    const int wide = read_half(source + 16u);
    const int narrow = read_half(source + 18u);
    const float at32 = static_cast<float>(wide);
    const float at28 = static_cast<float>(narrow);
    write_word(object + 32u, float_bits(at32));
    write_word(object + 28u, float_bits(at28));
    object[41] = static_cast<unsigned char>(cvt_w_s(at32));
    object[40] = static_cast<unsigned char>(cvt_w_s(at28));
    char* model = lumpFindResource__FPcT0(first, second);
    write_word(object + 88u, low_pointer(model));
    char* texture = lumpFindResource__FPcT0(
        static_cast<char*>(pointer_from_word(read_word(object + 76u))),
        static_cast<char*>(pointer_from_word(read_word(object + 84u))));
    write_word(object + 92u, low_pointer(texture));
    return 20;
}
