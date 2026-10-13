// Host check for playerMaybeGetItem__FiPQ24item11description (includes the source; build the file alone with -fno-builtin).
#define NORRATH_INVENTORY_HOST 1
#include "player_maybe_get_item.cpp"
#include <cstring>
#include <string>
#include <vector>
unsigned char player[2 * 23984];
const unsigned char norrath_inventory_ability_sound_lump_view[8] = {'f', 'x', '.', 'l', 'm', 'p', 0, 0};
static const void* table[8]; static unsigned used = 0;
static unsigned intern(const void* p) { table[used] = p; return 0x1000 + 0x10 * used++; }
const void* inventory_items::host_resolve(unsigned w) { return table[(w - 0x1000) / 0x10]; }
static std::vector<std::string> calls; static int feat; static float weight; static void* played_object; static std::string played;
extern "C" int sprintf(char* b, const char* f, ...) { std::strcpy(b, f); return int(std::strlen(f)); }
extern "C" int playerGetFeatRank__Fix(int p, long long f) { calls.push_back("feat" + std::to_string(p) + ":" + std::to_string(f)); return feat; }
extern "C" float getWeight__4itemPQ24item11description(const void*) { calls.push_back("weight"); return weight; }
extern "C" char* lumpFindResource__FPcT0(char* l, char* n) { calls.push_back(std::string("lump:") + l + ":" + n); return n; }
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* s, void* o, float v) { calls.push_back(std::string("play") + (v == 1.0f ? "1" : "?")); played_object = o; played = (const char*)s; }
extern "C" void scriptStart__FPc(char* n) { calls.push_back(std::string("script:") + n); }
extern "C" void addItemToInventory__FiPQ24item11descriptioni(int p, const void*, int c) { calls.push_back("add" + std::to_string(p) + ":" + std::to_string(c)); }
extern "C" int playerRecomputeWeight__Fi(int p) { calls.push_back("recompute" + std::to_string(p)); return 0; }
extern "C" void playerPlaySound__Fii(int p, int s) { calls.push_back("sound" + std::to_string(p) + ":" + std::to_string(s)); }
static unsigned char row[40], item[72];
static unsigned char* rec() { return player + 23984; }
static float& cur() { return *reinterpret_cast<float*>(rec() + 308); }
static float& cap() { return *reinterpret_cast<float*>(rec() + 312); }
static void setup(int type, int kind, unsigned flags) {
    std::memset(player, 0, sizeof player); std::memset(row, 0, sizeof row); std::memset(item, 0, sizeof item);
    row[28] = (unsigned char)type; row[29] = (unsigned char)kind; std::memcpy(item + 8, &flags, 4);
    static unsigned row_word = intern(row); std::memcpy(item, &row_word, 4);
    cur() = 10; cap() = 100; weight = 20; feat = 0; calls.clear();
    *reinterpret_cast<unsigned*>(rec() + 21872) = 0x1234;
}
int main() {
    setup(1, 0, 0);
    if (playerMaybeGetItem__FiPQ24item11description(1, item) != 1) return 1;
    if (calls != std::vector<std::string>{"weight", "lump:fx.lmp:inv_equip_jewelry.anm", "play1", "script:*gotItemNotify", "add1:1", "recompute1"}) return 2;
    if (played_object != reinterpret_cast<void*>(0x1234)) return 3;
    // type 8 asks for the feat, which halves the weight: 10 + 20 * 0.5 = 20 <= 100 and exactly at the cap below
    setup(8, 0, 0); feat = 1; cur() = 90; weight = 20; cap() = 100;
    if (playerMaybeGetItem__FiPQ24item11description(1, item) != 1 || calls[0] != "feat1:65536") return 4;
    setup(8, 0, 0); feat = 0; cur() = 90; weight = 20; cap() = 100;
    if (playerMaybeGetItem__FiPQ24item11description(1, item) != 0) return 5;       // 110 > 100
    if (calls != std::vector<std::string>{"feat1:65536", "weight", "sound1:0"}) return 6;
    setup(1, 0, 0); cur() = 80; weight = 20; cap() = 100;                           // exactly equal is accepted
    if (playerMaybeGetItem__FiPQ24item11description(1, item) != 1) return 7;
    setup(1, 0, 0); cur() = 1e30f; if (playerMaybeGetItem__FiPQ24item11description(1, item) != 0) return 8;
    // full list: count + 1 >= 64
    setup(1, 2, 0); *reinterpret_cast<int*>(rec() + 328 + 4616 * 2 + 4608) = 63;
    if (playerMaybeGetItem__FiPQ24item11description(1, item) != 0) return 9;
    if (calls != std::vector<std::string>{"weight", "lump:fx.lmp:game_butn_cantdo.anm", "play1"} || played_object != 0) return 10;
    setup(1, 2, 0); *reinterpret_cast<int*>(rec() + 328 + 4616 * 2 + 4608) = 62; *reinterpret_cast<int*>(rec() + 328 + 4616 * 1 + 4608) = 63;
    if (playerMaybeGetItem__FiPQ24item11description(1, item) != 1) return 11;       // only the item's own list counts
    // names
    const struct { unsigned flags; const char* name; } names[] = {
        {0x800000, "inv_equip_mdwp_wd.anm"}, {0x200000, "inv_equip_chain.anm"}, {0x40000, "inv_equip_mdwp_mtl.anm"},
        {0x100000, "inv_equip_leather.anm"}, {0x80000, "inv_equip_cloth.anm"}, {0x1000000, "inv_equip_potion.anm"},
        {0x4000000, "inv_equip_jewelry.anm"}, {0x2000000, "inv_equip_jewelry.anm"}, {0x10000000, "inv_equip_jewelry.anm"},
        {0x800000 | 0x40000, "inv_equip_mdwp_wd.anm"}, {0x40000 | 0x100000, "inv_equip_mdwp_mtl.anm"}, {0x80000 | 0x1000000, "inv_equip_cloth.anm"}};
    for (const auto& n : names) {
        setup(1, 0, n.flags); played.clear();
        if (playerMaybeGetItem__FiPQ24item11description(1, item) != 1 || played != n.name) return 12;
    }
    return 0;
}
