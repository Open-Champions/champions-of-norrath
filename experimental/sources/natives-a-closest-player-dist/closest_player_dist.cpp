// amxClosestPlayerDist is the 224-byte native at 0x00138c28. Cell 1 names an
// object. The result starts at the 100000 constant 0x47c35000. For each of
// nmPlayersAlive indexes in playersAlive, the word at byte 21872 of that
// player record is an entity. A null entity is skipped. Otherwise the
// distance from object+16 to entity+16 replaces the result when it is
// smaller. The final value is converted with cvt.w.s under round-to-nearest,
// ties to even. player records start at 0x00408600 and are 23984 bytes apart.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* destination, int* source);
extern "C" unsigned char* objectFindObject__FPc(char* name);
extern "C" int nmPlayersAlive;
extern "C" int playersAlive[];
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
    if (exponent_field == 255u || exponent_field == 0u) {
        if (exponent_field == 255u) return sign != 0 ? static_cast<int>(0x80000000u) : 0x7fffffff;
        return 0;
    }
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
extern "C" int amxClosestPlayerDist__FP5__amxPi(void* amx, int* params) {
    int* cell = 0;
    char name[160];
    amx_GetAddr(amx, params[1], &cell);
    amx_GetString(name, cell);
    unsigned char* object = objectFindObject__FPc(name);
    float best = 100000.0f;
    int count = nmPlayersAlive;
    if (count > 0) {
        int cursor = 0;
        do {
            unsigned index = static_cast<unsigned>(playersAlive[cursor]);
            unsigned char* entity = pointer_at(player + index * 23984u + 21872u);
            if (entity != 0) {
                float dx = load_f32(entity + 16) - load_f32(object + 16);
                float dy = load_f32(entity + 20) - load_f32(object + 20);
                float dz = load_f32(entity + 24) - load_f32(object + 24);
                float distance = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
                if (distance < best) best = distance;
            }
            cursor += 1;
            count -= 1;
        } while (count != 0);
    }
    return cvt_w_s(best);
}
