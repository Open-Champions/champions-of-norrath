// Observed amxHitPlayers at 0x00138a78, 428 bytes. It finds the object
// named by the first AMX cell. nmPlayersAlive words, starting at byte
// 5344 of nodes, are player indexes. A player is hit when the distance
// from object+16 to that creature's position at +16 is below the
// second cell. The hit calls the function at vtable+20 with the
// creature adjusted by the halfword at vtable+16, two zeros, the
// position, and the third cell as a float. It then stores the fourth
// cell as a float at creature+756 and the iatan2 of the y and x deltas
// as a halfword at creature+748. A miss does not reload the count.
extern "C" int nmPlayersAlive;
extern "C" unsigned char norrath_route_nodes_bytes[];
extern "C" unsigned char norrath_player_progression_records_view[];
extern "C" int amx_GetAddr(void* amx, int address, void** physical);
extern "C" int amx_GetString(char* destination, void* cells);
extern "C" void* objectFindObject__FPc(char* name);
extern "C" int iatan2__Fff(float y, float x);

using HitCall = void (*)(void* self, int zero_a, int zero_b, void* position, float damage);

static float float_at(const unsigned char* bytes) {
    union Bits { unsigned word; float value; } bits;
    bits.word = static_cast<unsigned>(bytes[0])
        | (static_cast<unsigned>(bytes[1]) << 8)
        | (static_cast<unsigned>(bytes[2]) << 16)
        | (static_cast<unsigned>(bytes[3]) << 24);
    return bits.value;
}
static void store_float(unsigned char* bytes, float value) {
    union Bits { unsigned word; float value; } bits;
    bits.value = value;
    bytes[0] = static_cast<unsigned char>(bits.word);
    bytes[1] = static_cast<unsigned char>(bits.word >> 8);
    bytes[2] = static_cast<unsigned char>(bits.word >> 16);
    bytes[3] = static_cast<unsigned char>(bits.word >> 24);
}
static void store_half(unsigned char* bytes, unsigned short value) {
    bytes[0] = static_cast<unsigned char>(value);
    bytes[1] = static_cast<unsigned char>(value >> 8);
}
static void* load_ptr(const unsigned char* bytes) {
    void* value;
    __builtin_memcpy(&value, bytes, sizeof value);
    return value;
}
static int word_at(const unsigned char* bytes) {
    return static_cast<int>(static_cast<unsigned>(bytes[0])
        | (static_cast<unsigned>(bytes[1]) << 8)
        | (static_cast<unsigned>(bytes[2]) << 16)
        | (static_cast<unsigned>(bytes[3]) << 24));
}

extern "C" int amxHitPlayers__FP5__amxPi(void* amx, int* params) {
    char text[160];
    void* cells = nullptr;
    (void)amx_GetAddr(amx, params[1], &cells);
    (void)amx_GetString(text, cells);
    unsigned char* object = static_cast<unsigned char*>(objectFindObject__FPc(text));
    int count = nmPlayersAlive;
    int index = 0;
    if (count <= 0) return 0;
    unsigned char* indexes = norrath_route_nodes_bytes + 5344;
    do {
        const int slot = word_at(indexes + index * 4);
        const float radius = static_cast<float>(params[2]);
        unsigned char* record = norrath_player_progression_records_view + slot * 23984;
        unsigned char* creature = static_cast<unsigned char*>(load_ptr(record + 21872));
        const float dx = float_at(creature + 16) - float_at(object + 16);
        const float dy = float_at(creature + 20) - float_at(object + 20);
        const float dz = float_at(creature + 24) - float_at(object + 24);
        const float distance = __builtin_sqrtf((dx * dx) + (dy * dy) + (dz * dz));
        if (distance < radius) {
            unsigned char* table = static_cast<unsigned char*>(load_ptr(creature + 72));
            short adjust = 0;
            __builtin_memcpy(&adjust, table + 16, sizeof adjust);
            HitCall call = nullptr;
            __builtin_memcpy(&call, table + 20, sizeof call);
            call(creature + adjust, 0, 0, creature + 16, static_cast<float>(params[3]));
            const int stored = word_at(indexes + index * 4);
            unsigned char* struck = static_cast<unsigned char*>(load_ptr(
                norrath_player_progression_records_view + stored * 23984 + 21872));
            store_float(struck + 756, static_cast<float>(params[4]));
            const int angle = iatan2__Fff(dy, dx);
            count = nmPlayersAlive;
            const int after = word_at(indexes + index * 4);
            struck = static_cast<unsigned char*>(load_ptr(
                norrath_player_progression_records_view + after * 23984 + 21872));
            store_half(struck + 748, static_cast<unsigned short>(angle));
        }
        index += 1;
    } while (index < count);
    return 0;
}
