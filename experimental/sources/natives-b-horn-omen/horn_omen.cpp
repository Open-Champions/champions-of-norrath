// Observed amxHornOmen at 0x0013bcf8, 168 bytes. For each nmPlayers
// record whose creature word at player+21872 is nonzero, it allocates
// 32 bytes, constructs SpellHornOmenEffect when that block exists, and
// always passes the block to addSpell. The skill argument is skills
// slot 29, the pointer at byte 116 of the 4-byte catalog. A null
// creature does not reload nmPlayers. The record stride is 23984.
extern "C" unsigned char nmPlayers[4];
extern "C" unsigned char norrath_player_progression_records_view[];
extern "C" unsigned char* norrath_player_skill_catalog_view[];
extern "C" void* blockAlloc__Fi(int bytes);
extern "C" void __19SpellHornOmenEffectiP8CreatureP5Skill(void* block, int rank, void* creature, void* skill);
extern "C" void addSpell__8CreatureP5Spell(void* creature, void* spell);

static int signed_word(const unsigned char* bytes) {
    const unsigned word = static_cast<unsigned>(bytes[0])
        | (static_cast<unsigned>(bytes[1]) << 8)
        | (static_cast<unsigned>(bytes[2]) << 16)
        | (static_cast<unsigned>(bytes[3]) << 24);
    return static_cast<int>(word);
}
static void* load_ptr(const unsigned char* bytes) {
    void* value;
    __builtin_memcpy(&value, bytes, sizeof value);
    return value;
}

extern "C" int amxHornOmen__FP5__amxPi(void*, int*) {
    int count = signed_word(nmPlayers);
    int index = 0;
    if (count > 0) {
        unsigned char* record = norrath_player_progression_records_view;
        do {
            void* creature = load_ptr(record + 21872);
            if (creature == nullptr) {
                index += 1;
            } else {
                void* block = blockAlloc__Fi(32);
                if (block != nullptr) {
                    __19SpellHornOmenEffectiP8CreatureP5Skill(
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
