// amxGetPlayerVDir is the 256-byte native at 0x00138f20. It walks
// nmPlayersAlive indexes in playersAlive. The entity word is at byte 21872
// of the player record. Bit 0x8 of entity flags, a null entity, a null word
// at entity byte 1328, or a vehicle float at byte 840 that is not greater
// than 2 skips that player. Otherwise the dot of the negated floats at
// vehicle bytes 1128, 1132, and 1136 with the three floats 12 bytes after
// the pointer at vehicle byte 2320 decides the result: greater than zero
// returns 1, and any other ordered value returns -1. No accepted player
// returns zero.
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
static float dot_threshold() {
    union {
        unsigned as_bits;
        float as_float;
    } value;
    value.as_bits = 0x3f34fdf3u;
    return value.as_float;
}
extern "C" int amxGetPlayerVDir__FP5__amxPi(void* amx, int* params) {
    (void)amx;
    (void)params;
    int count = nmPlayersAlive;
    if (count <= 0) return 0;
    const float threshold = dot_threshold();
    for (int cursor = 0; cursor < count; ++cursor) {
        unsigned index = static_cast<unsigned>(playersAlive[cursor]);
        unsigned char* entity = pointer_at(player + index * 23984u + 21872u);
        if (entity == 0) continue;
        if ((load_u32(entity + 12) & 8u) != 0u) continue;
        unsigned char* vehicle = pointer_at(entity + 1328);
        if (vehicle == 0) continue;
        if (!(2.0f < load_f32(vehicle + 840))) continue;
        float x = -load_f32(vehicle + 1128);
        float y = -load_f32(vehicle + 1132);
        float z = -load_f32(vehicle + 1136);
        unsigned char* basis = pointer_at(vehicle + 2320) + 12;
        float dot = x * load_f32(basis) + y * load_f32(basis + 4) + z * load_f32(basis + 8);
        if (0.0f < dot) return 1;
        if (dot < threshold) return -1;
    }
    return 0;
}
