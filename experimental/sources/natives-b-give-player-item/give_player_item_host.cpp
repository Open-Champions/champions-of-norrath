#include <cstdio>
#include <cstring>
extern "C" int amxGivePlayerItem__FP5__amxPi(void*, int*);
extern "C" unsigned char nmPlayers[4] = {};
extern "C" unsigned char norrath_player_progression_records_view[23984 * 2] = {};
static unsigned char item[32];
static unsigned char creature[28];
static int creates = 0;
static int feats = 0;
static int adds = 0;
static int equips = 0;
static int props = 0;
static int feat_rank = 0;
static void* equipped = nullptr;
static char created_name[32];
static int created_kind = 0;
static int created_flags = 0;
static unsigned char description_32 = 0;
static unsigned char description_16 = 0;
extern "C" int amx_GetAddr(void*, int, void** physical) {
    static char packed[4];
    *physical = packed;
    return 0;
}
extern "C" int amx_GetString(char* destination, void*) {
    std::strcpy(destination, "gem");
    return 0;
}
extern "C" void create__4itemPcPQ24item11descriptionii(char* name, void* description, int kind, int flags) {
    creates += 1;
    std::strcpy(created_name, name);
    created_kind = kind;
    created_flags = flags;
    const unsigned char* bytes = static_cast<const unsigned char*>(description);
    description_32 = bytes[32];
    description_16 = bytes[16];
    if (bytes[36] != 0xff || bytes[60] != 0xff) creates = -100;
    *reinterpret_cast<void**>(description) = item;
}
extern "C" int playerGetFeatRank__Fix(int index, int feat) {
    feats += 1;
    if (index != 0 || feat != 0x10000) return -1;
    return feat_rank;
}
extern "C" float getWeight__4itemPQ24item11description(void*) { return 10.f; }
extern "C" int addItemToInventory__FiPQ24item11descriptioni(int, void*, int count) {
    adds += 1;
    if (count != 1) adds = -100;
    nmPlayers[0] = 1;
    return 2;
}
extern "C" void playerRecomputeWeight__Fi(int) {}
extern "C" void equipItem__FiPQ24item11descriptioni(int index, void* slot, int flags) {
    equips += 1;
    if (index != 0 || flags != 0) equips = -100;
    equipped = slot;
}
extern "C" void* blockAlloc__Fi(int bytes) { return bytes == 400 ? item : nullptr; }
extern "C" void __8ItemPropG6Point3PQ24item11descriptioni(void*, void* point, void*, int) {
    props += 1;
    if (std::memcmp(point, creature + 16, 12) != 0) props = -100;
    nmPlayers[0] = 1;
}
static void put_float(unsigned char* bytes, float value) {
    union Bits { unsigned word; float value; } bits;
    bits.value = value;
    std::memcpy(bytes, &bits.word, 4);
}

int main() {
    std::memset(item, 0, sizeof item);
    std::memset(creature, 0x5a, sizeof creature);
    std::memset(nmPlayers, 0, 4);
    int plain[] = {8, 1, 0};
    if (amxGivePlayerItem__FP5__amxPi(nullptr, plain) != 0) return 1;
    if (creates != 0) return 2;
    nmPlayers[0] = 2;
    item[28] = 1;
    put_float(norrath_player_progression_records_view + 308, 0.f);
    put_float(norrath_player_progression_records_view + 312, 8.f);
    *reinterpret_cast<void**>(norrath_player_progression_records_view + 21872) = creature;
    if (amxGivePlayerItem__FP5__amxPi(nullptr, plain) != 0) return 3;
    if (creates != 1 || props != 1 || adds != 0 || feats != 0) return 4;
    if (std::strcmp(created_name, "gem") != 0 || created_kind != -1 || created_flags != 0) return 5;
    if (description_32 != 0xff || description_16 != 0) return 6;
    item[28] = 8;
    feat_rank = 4;
    put_float(norrath_player_progression_records_view + 312, 8.f);
    nmPlayers[0] = 1;
    int equip[] = {12, 1, 1};
    item[29] = 1;
    if (amxGivePlayerItem__FP5__amxPi(nullptr, equip) != 0) return 7;
    if (feats != 1 || adds != 1 || equips != 1 || props != 1) return 8;
    if (equipped != norrath_player_progression_records_view + 328 + 4616 + 2 * 72) return 9;
    std::puts("amxGivePlayerItem passed");
    return 0;
}
