#include <cstdio>
#include <cstring>
extern "C" int amxGivePlayerRandomItem__FP5__amxPi(void*, int*);
extern "C" unsigned char nmPlayers[4] = {};
extern "C" unsigned char norrath_player_progression_records_view[23984] = {};
extern "C" unsigned char norrath_constructor_random_state_view[4] = {};
extern "C" unsigned char levelName[322] = {};
extern "C" int gameDifficulty = 1;
extern "C" float worldDifficulty = 1.5f;
static int swords = 0;
static int weights = 0;
static int randoms = 0;
static int adds = 0;
static int rolled_seen = -1;
static int bit_seen = -1;
static int difficulty_seen = -1;
static int kind_seen = 0;
static unsigned char byte_32 = 0;
static unsigned char byte_33 = 0;
static char sword_name[96];
static int random_result = 1;
extern "C" float gameItemTreasureRamp__Fv(void) { return 1.f; }
extern "C" void create__4itemPcPQ24item11descriptionii(char* name, void*, int, int) {
    swords += 1;
    std::strcpy(sword_name, name);
}
extern "C" float getWeight__4itemPQ24item11description(void* description) {
    weights += 1;
    const unsigned char* bytes = static_cast<const unsigned char*>(description);
    byte_32 = bytes[32];
    byte_33 = bytes[33];
    return 1.f;
}
extern "C" int addItemToInventory__FiPQ24item11descriptioni(int, void*, int) {
    adds += 1;
    return 0;
}
extern "C" void playerRecomputeWeight__Fi(int) {}
extern "C" void* blockAlloc__Fi(int) { return nullptr; }
extern "C" void __8ItemPropG6Point3PQ24item11descriptioni(void*, void*, void*, int) {}
extern "C" int createRandom__4itemPQ24item11descriptioniiii(void*, int rolled, int bit, int kind, int difficulty) {
    randoms += 1;
    rolled_seen = rolled;
    bit_seen = bit;
    kind_seen = kind;
    difficulty_seen = difficulty;
    return random_result;
}
static void put_float(unsigned char* bytes, float value) {
    union Bits { unsigned word; float value; } bits;
    bits.value = value;
    std::memcpy(bytes, &bits.word, 4);
}
static void reset_counts() {
    swords = 0;
    weights = 0;
    randoms = 0;
    adds = 0;
    std::memset(norrath_constructor_random_state_view, 0, 4);
}

int main() {
    put_float(norrath_player_progression_records_view + 308, 0.f);
    put_float(norrath_player_progression_records_view + 312, 100.f);
    std::memset(nmPlayers, 0, 4);
    if (amxGivePlayerRandomItem__FP5__amxPi(nullptr, nullptr) != 0) return 1;
    if (swords != 0 || randoms != 0) return 2;
    nmPlayers[0] = 1;
    std::strcpy(reinterpret_cast<char*>(levelName), "orccave1");
    reset_counts();
    if (amxGivePlayerRandomItem__FP5__amxPi(nullptr, nullptr) != 0) return 3;
    if (swords != 0 || weights != 1 || randoms != 1 || adds != 1) return 4;
    if (rolled_seen != 232 || bit_seen != 0 || kind_seen != -1 || difficulty_seen != 2) return 5;
    std::memset(levelName, 0, sizeof levelName);
    std::strcpy(reinterpret_cast<char*>(levelName + 65), "orccave1");
    gameDifficulty = 1;
    reset_counts();
    if (amxGivePlayerRandomItem__FP5__amxPi(nullptr, nullptr) != 0) return 6;
    if (swords != 1 || weights != 2 || adds != 2 || byte_32 != 18 || byte_33 != 51) return 7;
    if (std::strcmp(sword_name, "shortsword+damageincrease75+weightmodifier-25+requiredlevel5+dexteritybonus5") != 0) return 8;
    random_result = 0;
    reset_counts();
    if (amxGivePlayerRandomItem__FP5__amxPi(nullptr, nullptr) != 0) return 9;
    if (randoms != 10) return 10;
    std::puts("amxGivePlayerRandomItem passed");
    return 0;
}
