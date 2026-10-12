// Observed amxGivePlayerRandomItem at 0x00139e10, 880 bytes. It does
// not read the AMX cells. For each nmPlayers record it steps
// syncRandState by multiplying by 0x0019660d and adding 0x3c6ef35f,
// then rounds the high half times gameItemTreasureRamp times the word
// 0x3c75c28f. When levelName+65 contains "orccave1" and gameDifficulty
// is 1, 2, or 3, it creates the matching sword and stores the two
// observed bytes, then applies the weight test. It then calls
// createRandom up to ten times, stopping on a nonzero result, and
// applies the weight test again. Ties in the integer conversion round
// to even. Stride 23984. The description stores match amxGivePlayerItem;
// the other bytes of this buffer are cleared.
extern "C" unsigned char nmPlayers[4];
extern "C" unsigned char norrath_player_progression_records_view[];
extern "C" unsigned char norrath_constructor_random_state_view[4];
extern "C" unsigned char levelName[];
extern "C" int gameDifficulty;
extern "C" float worldDifficulty;
extern "C" char* strstr(const char* text, const char* needle);
extern "C" float gameItemTreasureRamp__Fv(void);
extern "C" void create__4itemPcPQ24item11descriptionii(char* name, void* description, int kind, int flags);
extern "C" float getWeight__4itemPQ24item11description(void* description);
extern "C" int addItemToInventory__FiPQ24item11descriptioni(int index, void* description, int count);
extern "C" void playerRecomputeWeight__Fi(int index);
extern "C" void* blockAlloc__Fi(int bytes);
extern "C" void __8ItemPropG6Point3PQ24item11descriptioni(void* block, void* point, void* description, int flags);
extern "C" int createRandom__4itemPQ24item11descriptioniiii(void* description, int rolled, int bit, int kind, int difficulty);

static const char short_sword[] = "shortsword+damageincrease75+weightmodifier-25+requiredlevel5+dexteritybonus5";
static const char light_sword[] = "lightsword+damageincrease460+weightmodifier25+requiredlevel21+dexteritybonus10";
static const char falchion[] = "falchion+damageincrease350+requiredlevel33+dexteritybonus15";

static unsigned word_at(const unsigned char* bytes) {
    return static_cast<unsigned>(bytes[0])
        | (static_cast<unsigned>(bytes[1]) << 8)
        | (static_cast<unsigned>(bytes[2]) << 16)
        | (static_cast<unsigned>(bytes[3]) << 24);
}
static void store_word(unsigned char* bytes, unsigned value) {
    bytes[0] = static_cast<unsigned char>(value);
    bytes[1] = static_cast<unsigned char>(value >> 8);
    bytes[2] = static_cast<unsigned char>(value >> 16);
    bytes[3] = static_cast<unsigned char>(value >> 24);
}
static float float_at(const unsigned char* bytes) {
    union Bits { unsigned word; float value; } bits;
    bits.word = word_at(bytes);
    return bits.value;
}
static void* load_ptr(const unsigned char* bytes) {
    void* value;
    __builtin_memcpy(&value, bytes, sizeof value);
    return value;
}
static int round_even(float value) {
    const int negative = value < 0.f;
    const float magnitude = negative ? -value : value;
    const int whole = static_cast<int>(magnitude);
    const float fraction = magnitude - static_cast<float>(whole);
    int rounded = whole;
    if (fraction > 0.5f || (fraction == 0.5f && (whole & 1) != 0)) rounded = whole + 1;
    return negative ? -rounded : rounded;
}
static unsigned step_state(void) {
    unsigned state = word_at(norrath_constructor_random_state_view);
    state = state * 0x0019660du + 0x3c6ef35fu;
    store_word(norrath_constructor_random_state_view, state);
    return state;
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
static void settle(unsigned char* description, unsigned char* record, int index) {
    const float weight = getWeight__4itemPQ24item11description(description);
    if (float_at(record + 308) + weight < float_at(record + 312)) {
        (void)addItemToInventory__FiPQ24item11descriptioni(index, description, 1);
        playerRecomputeWeight__Fi(index);
    } else {
        void* block = blockAlloc__Fi(400);
        if (block != nullptr) {
            unsigned char point[12];
            unsigned char* creature = static_cast<unsigned char*>(load_ptr(record + 21872));
            __builtin_memcpy(point, creature + 16, 12);
            __8ItemPropG6Point3PQ24item11descriptioni(block, point, description, 0);
        }
    }
}

extern "C" int amxGivePlayerRandomItem__FP5__amxPi(void*, int*) {
    int count = word_at(nmPlayers);
    if (static_cast<int>(count) <= 0) return 0;
    union Bits { unsigned word; float value; } scale;
    scale.word = 0x3c75c28fu;
    int index = 0;
    unsigned char* record = norrath_player_progression_records_view;
    do {
        unsigned char description[80];
        prepare(description);
        const unsigned high = step_state() >> 16;
        const float ramp = gameItemTreasureRamp__Fv();
        const int rolled = round_even((static_cast<float>(high) * ramp) * scale.value);
        if (strstr(reinterpret_cast<char*>(levelName + 65), "orccave1") != nullptr) {
            const char* sword = nullptr;
            if (gameDifficulty == 1) sword = short_sword;
            else if (gameDifficulty == 2) sword = light_sword;
            else if (gameDifficulty == 3) sword = falchion;
            if (sword != nullptr) {
                create__4itemPcPQ24item11descriptionii(const_cast<char*>(sword), description, 0, 0);
                if (gameDifficulty == 1) {
                    description[32] = 18;
                    description[33] = 51;
                } else if (gameDifficulty == 2) {
                    description[32] = 51;
                    description[33] = 52;
                } else {
                    description[32] = 36;
                    description[33] = 53;
                }
            }
            settle(description, record, index);
        }
        for (int attempt = 0; attempt < 10; ++attempt) {
            const unsigned next = step_state();
            const int bit = static_cast<int>((next >> 16) & 1u);
            if (createRandom__4itemPQ24item11descriptioniiii(
                    description, rolled, bit, -1, round_even(worldDifficulty)) != 0) {
                break;
            }
        }
        settle(description, record, index);
        index += 1;
        count = word_at(nmPlayers);
        record += 23984;
    } while (index < static_cast<int>(count));
    return 0;
}
