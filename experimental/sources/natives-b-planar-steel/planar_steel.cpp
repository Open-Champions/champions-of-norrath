// Observed amxPlanarSteel at 0x0013a180, 204 bytes. For each current
// nmPlayers record it copies the float at player+176 over the float at
// player+172 when that hit-point float is above zero. A nonzero
// creature word at player+21872 then gets a 108-byte SpellPlanarSteelEffect.
// addSpell still runs when the block is null. The skill argument is
// skills slot 29. A null creature does not reload nmPlayers. Stride 23984.
extern "C" unsigned char nmPlayers[4];
extern "C" unsigned char norrath_player_progression_records_view[];
extern "C" unsigned char* norrath_player_skill_catalog_view[];
extern "C" void* blockAlloc__Fi(int bytes);
extern "C" void __22SpellPlanarSteelEffectiP8CreatureP5Skill(void* block, int rank, void* creature, void* skill);
extern "C" void addSpell__8CreatureP5Spell(void* creature, void* spell);

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
static void store_float(unsigned char* bytes, float value) {
    union Bits { unsigned word; float value; } bits;
    bits.value = value;
    bytes[0] = static_cast<unsigned char>(bits.word);
    bytes[1] = static_cast<unsigned char>(bits.word >> 8);
    bytes[2] = static_cast<unsigned char>(bits.word >> 16);
    bytes[3] = static_cast<unsigned char>(bits.word >> 24);
}
static void* load_ptr(const unsigned char* bytes) {
    void* value;
    __builtin_memcpy(&value, bytes, sizeof value);
    return value;
}

extern "C" int amxPlanarSteel__FP5__amxPi(void*, int*) {
    int count = signed_word(nmPlayers);
    int index = 0;
    if (count > 0) {
        unsigned char* record = norrath_player_progression_records_view;
        do {
            const float hp = float_at(record + 172);
            if (0.f < hp) store_float(record + 172, float_at(record + 176));
            void* creature = load_ptr(record + 21872);
            if (creature == nullptr) {
                index += 1;
            } else {
                void* block = blockAlloc__Fi(108);
                if (block != nullptr) {
                    __22SpellPlanarSteelEffectiP8CreatureP5Skill(
                        block, 1, creature, norrath_player_skill_catalog_view[29]);
                }
                addSpell__8CreatureP5Spell(creature, block);
                count = signed_word(nmPlayers);
                index += 1;
            }
            record += 23984;
        } while (index < count);
    }
    return 0;
}
