// Observed amxGivePlayerItem at 0x00139bc8, 580 bytes. For each
// nmPlayers record it builds an item description, creates the named
// item with arguments -1 and 0, and scales the weight by one half when
// byte 28 of the created item is 8 and playerGetFeatRank of 0x10000 is
// nonzero. The scaled weight is added to the float at player+308. Below
// the float at player+312, the item is added and the weight recomputed;
// a nonzero second AMX cell also equips it. Otherwise a 400-byte
// ItemProp is built from the creature position at player+21872. The
// original description stores are the two 0xFF bytes at 32 and 33, the
// zero halfwords at 16 through 31, and the 0xFFFF halfwords at 36, 44,
// 52, and 60. This buffer clears the other bytes. Stride 23984.
extern "C" unsigned char nmPlayers[4];
extern "C" unsigned char norrath_player_progression_records_view[];
extern "C" int amx_GetAddr(void* amx, int address, void** physical);
extern "C" int amx_GetString(char* destination, void* cells);
extern "C" void create__4itemPcPQ24item11descriptionii(char* name, void* description, int kind, int flags);
extern "C" int playerGetFeatRank__Fix(int index, int feat);
extern "C" float getWeight__4itemPQ24item11description(void* description);
extern "C" int addItemToInventory__FiPQ24item11descriptioni(int index, void* description, int count);
extern "C" void playerRecomputeWeight__Fi(int index);
extern "C" void equipItem__FiPQ24item11descriptioni(int index, void* slot, int flags);
extern "C" void* blockAlloc__Fi(int bytes);
extern "C" void __8ItemPropG6Point3PQ24item11descriptioni(void* block, void* point, void* description, int flags);

static int signed_word(const unsigned char* bytes) {
    const unsigned word = static_cast<unsigned>(bytes[0])
        | (static_cast<unsigned>(bytes[1]) << 8)
        | (static_cast<unsigned>(bytes[2]) << 16)
        | (static_cast<unsigned>(bytes[3]) << 24);
    return static_cast<int>(word);
}
static float float_at(const unsigned char* bytes) {
    union Bits { unsigned word; float value; } bits;
    bits.word = static_cast<unsigned>(signed_word(bytes));
    return bits.value;
}
static void* load_ptr(const unsigned char* bytes) {
    void* value;
    __builtin_memcpy(&value, bytes, sizeof value);
    return value;
}
static void prepare(unsigned char* description) {
    for (int offset = 0; offset < 80; ++offset) description[offset] = 0;
    description[32] = 0xff;
    description[33] = 0xff;
    for (int offset = 36; offset <= 60; offset += 8) {
        description[offset] = 0xff;
        description[offset + 1] = 0xff;
    }
}

extern "C" int amxGivePlayerItem__FP5__amxPi(void* amx, int* params) {
    char name[160];
    void* cells = nullptr;
    (void)amx_GetAddr(amx, params[1], &cells);
    (void)amx_GetString(name, cells);
    int count = signed_word(nmPlayers);
    int index = 0;
    if (count <= 0) return 0;
    unsigned char* record = norrath_player_progression_records_view;
    do {
        unsigned char description[80];
        prepare(description);
        create__4itemPcPQ24item11descriptionii(name, description, -1, 0);
        unsigned char* item = static_cast<unsigned char*>(load_ptr(description));
        float scale = 1.f;
        if (item[28] == 8 && playerGetFeatRank__Fix(index, 0x10000) != 0) scale = 0.5f;
        const float weight = getWeight__4itemPQ24item11description(description) * scale;
        if (float_at(record + 308) + weight < float_at(record + 312)) {
            const int slot = addItemToInventory__FiPQ24item11descriptioni(index, description, 1);
            playerRecomputeWeight__Fi(index);
            if (params[2] != 0) {
                const int kind = static_cast<signed char>(item[29]);
                equipItem__FiPQ24item11descriptioni(
                    index, record + 328 + kind * 4616 + slot * 72, 0);
            }
        } else {
            void* block = blockAlloc__Fi(400);
            if (block != nullptr) {
                unsigned char point[12];
                unsigned char* creature = static_cast<unsigned char*>(load_ptr(record + 21872));
                __builtin_memcpy(point, creature + 16, 12);
                __8ItemPropG6Point3PQ24item11descriptioni(block, point, description, 0);
            }
        }
        index += 1;
        count = signed_word(nmPlayers);
        record += 23984;
    } while (index < count);
    return 0;
}
