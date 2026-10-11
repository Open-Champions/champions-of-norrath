// amxGetHitPoints is the 88-byte native at 0x0013a918. Cell 1 names one
// object. A null object, or an object whose flags word at byte 12 lacks bit
// 0x20, returns zero. Otherwise the float at byte 560 is converted with
// cvt.w.s. This function does not change the rounding mode; the conversion
// below is round-to-nearest, ties to even, which is the MIPS reset mode.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* destination, int* source);
extern "C" unsigned char* objectFindObject__FPc(char* name);
static unsigned load_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}
static int cvt_w_s(float value) {
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
    if (shift >= 0) {
        magnitude = fraction << shift;
    } else {
        int places = -shift;
        unsigned base = fraction >> places;
        unsigned remainder = fraction & ((1u << places) - 1u);
        unsigned half = 1u << (places - 1);
        if (remainder > half || (remainder == half && (base & 1u) != 0u)) base += 1u;
        magnitude = base;
    }
    return sign != 0 ? -static_cast<int>(magnitude) : static_cast<int>(magnitude);
}
extern "C" int amxGetHitPoints__FP5__amxPi(void* amx, int* params) {
    int* cell = 0;
    char name[160];
    amx_GetAddr(amx, params[1], &cell);
    amx_GetString(name, cell);
    unsigned char* object = objectFindObject__FPc(name);
    if (object == 0) return 0;
    if ((load_u32(object + 12) & 0x20u) == 0u) return 0;
    union {
        unsigned as_bits;
        float as_float;
    } value;
    value.as_bits = load_u32(object + 560);
    return cvt_w_s(value.as_float);
}
