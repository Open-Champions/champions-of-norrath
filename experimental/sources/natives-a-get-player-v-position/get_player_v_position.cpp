// amxGetPlayerVPosition is the 132-byte native at 0x0013ad08. It walks
// nmPlayers records. The word at byte 21872 is an entity. Bit 0x8 of the
// flags word at entity byte 12, or a null word at entity byte 1328, skips
// the record. Otherwise the float at vehicle byte 2316 is compared with the
// running value, and a smaller running value is replaced by the float at
// vehicle byte 844. The running value starts at 0, is multiplied by 128, and
// is converted with cvt.w.s, round-to-nearest ties to even.
extern "C" int nmPlayers;
extern "C" unsigned char player[];
static unsigned load_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}
static float load_f32(const unsigned char* address) {
    union {
        unsigned as_bits;
        float as_float;
    } value;
    value.as_bits = load_u32(address);
    return value.as_float;
}
static unsigned char* pointer_at(const unsigned char* address) {
    unsigned word = load_u32(address);
#if defined(NORRATH_HOST_CONTRACT)
    if (word == 0u) return 0;
    extern unsigned char* norrath_native_host_base;
    return norrath_native_host_base + word;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
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
    if (exponent_field == 0u) return 0;
    if (exponent_field == 255u || static_cast<int>(exponent_field) - 127 >= 31)
        return sign != 0 ? static_cast<int>(0x80000000u) : 0x7fffffff;
    int exponent = static_cast<int>(exponent_field) - 127;
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
extern "C" int amxGetPlayerVPosition__FP5__amxPi(void* amx, int* params) {
    (void)amx;
    (void)params;
    float best = 0.0f;
    int count = nmPlayers;
    if (count > 0) {
        unsigned char* slot = player + 21872;
        do {
            unsigned char* entity = pointer_at(slot);
            slot += 23984;
            if (entity != 0 && (load_u32(entity + 12) & 8u) == 0u) {
                unsigned char* vehicle = pointer_at(entity + 1328);
                if (vehicle != 0) {
                    float probe = load_f32(vehicle + 2316);
                    if (best < probe) best = load_f32(vehicle + 844);
                }
            }
            count -= 1;
        } while (count != 0);
    }
    return cvt_w_s(best * 128.0f);
}
