// Host check for unequipItem__FiPQ24item11description (includes the source; build the file alone with -fno-builtin).
#define NORRATH_INVENTORY_HOST 1
#include "unequip_item.cpp"
#include <cstring>
#include <string>
#include <vector>
unsigned char player[2 * 23984];
const unsigned char norrath_inventory_ability_sound_lump_view[8] = {'f', 'x', '.', 'l', 'm', 'p', 0, 0};
unsigned char animState[2 * 264];
static const void* table[64]; static unsigned used = 0;
static unsigned intern(const void* p) { table[used] = p; return 0x1000 + 0x10 * used++; }
const void* inventory_items::host_resolve(unsigned w) { return table[(w - 0x1000) / 0x10]; }
static std::vector<std::string> calls; static int local = 1; static std::string played;
extern "C" int sprintf(char* b, const char* f, ...) { std::strcpy(b, f); return int(std::strlen(f)); }
extern "C" int netPlayerIsLocal__Fi(int) { calls.push_back("local"); return local; }
extern "C" void inventorySwitchDualWield__FiPQ24item11description(int p, const void*) { calls.push_back("dual" + std::to_string(p)); }
extern "C" void resetPlayerAnimations__Fi(int p) { calls.push_back("resetanim" + std::to_string(p)); }
extern "C" void inventorySetAnim__FiP14AnimationStatei(int p, void* s, int v) { calls.push_back("setanim" + std::to_string(p) + (s == animState + 264 * p ? "" : "!") + std::to_string(v)); }
extern "C" void playerRecomputeAbilityModifiers__Fv() { calls.push_back("recompute"); }
extern "C" char* lumpFindResource__FPcT0(char* l, char* n) { calls.push_back(std::string("lump:") + l + ":" + n); played = n; return n; }
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* s, void* o, float v) { calls.push_back(std::string("play") + (o == 0 ? "0" : "!") + (v == 1.0f ? "1" : "?")); (void)s; }
static unsigned& rw(unsigned char* b, unsigned o) { return *reinterpret_cast<unsigned*>(b + o); }
static unsigned char* rec() { return player + 23984; }
static unsigned char row[40], slot[72]; static unsigned slot_word;
static void setup(int kind, int type, unsigned flags) {
    std::memset(player, 0, sizeof player); std::memset(row, 0, sizeof row); std::memset(slot, 0, sizeof slot);
    row[29] = (unsigned char)kind; row[28] = (unsigned char)type; std::memcpy(slot + 8, &flags, 4);
    static unsigned row_word = intern(row); slot_word = inventory_items::word_of(slot);
    std::memcpy(slot, &row_word, 4); calls.clear();
}
int main() {
    // kind 0, plain type: nothing held -> clears the type word and finishes with the sound part
    setup(0, 2, 0); rw(rec(), 21896 + 8) = slot_word; rw(rec(), 21960 + 4) = slot_word; rw(rec(), 21960) = 0x77;
    unequipItem__FiPQ24item11description(1, slot);
    if (rw(rec(), 21896 + 8) != 0 || rw(rec(), 21964) != 0 || rw(rec(), 21960) != 0x77) return 1;
    if (calls != std::vector<std::string>{"local", "lump:fx.lmp:inv_drop_jewelry.anm", "play01", "setanim10", "recompute"}) return 2;
    // held at the selected index: animation reset first
    setup(0, 2, 0); rw(rec(), 21960 + 4) = slot_word; rec()[23572] = 1;
    unequipItem__FiPQ24item11description(1, slot);
    if (rec()[23582] != 5 || calls.size() < 3 || calls[0] != "resetanim1" || calls[1] != "setanim10") return 3;
    // type 8 never resets the animation
    setup(0, 8, 0); rw(rec(), 21960) = slot_word; rec()[23572] = 0;
    unequipItem__FiPQ24item11description(1, slot);
    if (rec()[23582] != 0 || calls[0] == "resetanim1" || rw(rec(), 21960) != 0) return 4;
    // dual wield: a non-zero word at +21968 and type 0
    setup(0, 0, 0x800000); rw(rec(), 21968) = 5; rw(rec(), 21896) = slot_word;
    unequipItem__FiPQ24item11description(1, slot);
    if (calls[0] != "dual1" || rw(rec(), 21896) != slot_word) return 5;
    if (played != "inv_drop_mdwp_wd.anm") return 6;
    setup(0, 4, 0); rw(rec(), 21968) = 5;                    // type not 0: normal path
    unequipItem__FiPQ24item11description(1, slot);
    if (calls[0] == "dual1") return 7;
    // kind 1
    setup(1, 3, 0); rw(rec(), 21896 + 12) = slot_word; rw(rec(), 21960) = slot_word;
    unequipItem__FiPQ24item11description(1, slot);
    if (rw(rec(), 21896 + 12) != 0 || rw(rec(), 21960) != slot_word || played != "inv_drop_sm_shield.anm") return 8;
    // kind 2 chain
    setup(2, 7, 0); rw(rec(), 21920) = slot_word; rw(rec(), 21916) = slot_word; rw(rec(), 21924) = 9;
    unequipItem__FiPQ24item11description(1, slot);
    if (rw(rec(), 21920) != 0 || rw(rec(), 21916) != slot_word || rw(rec(), 21924) != 9) return 9;
    setup(2, 7, 0); rw(rec(), 21916) = slot_word; rw(rec(), 21924) = 9;
    unequipItem__FiPQ24item11description(1, slot);
    if (rw(rec(), 21916) != 0 || rw(rec(), 21924) != 9) return 10;
    setup(2, 7, 0); rw(rec(), 21924) = 9; unequipItem__FiPQ24item11description(1, slot); if (rw(rec(), 21924) != 0) return 11;
    setup(2, 14, 0); rw(rec(), 21952) = 9; unequipItem__FiPQ24item11description(1, slot); if (rw(rec(), 21952) != 0) return 12;
    setup(2, 5, 0); rw(rec(), 21952) = 9; rw(rec(), 21924) = 9; unequipItem__FiPQ24item11description(1, slot); if (rw(rec(), 21952) != 9 || rw(rec(), 21924) != 9) return 13;
    setup(3, 7, 0); rw(rec(), 21896 + 28) = slot_word; unequipItem__FiPQ24item11description(1, slot); if (rw(rec(), 21896 + 28) != slot_word) return 14;
    // sound names by flag, with local player off
    const struct { unsigned flags; int type; const char* name; } names[] = {
        {0x800000, 0, "inv_drop_mdwp_wd.anm"}, {0x200000, 0, "inv_drop_chain.anm"}, {0, 3, "inv_drop_sm_shield.anm"},
        {0x400000, 0, "inv_drop_plate.anm"}, {0x40000, 0, "inv_drop_mdwp_mtl.anm"}, {0x100000, 0, "inv_drop_leather.anm"},
        {0x80000, 0, "inv_drop_cloth.anm"}, {0x1000000, 0, "inv_drop_potion.anm"}, {0x4000000, 0, "inv_drop_jewelry.anm"},
        {0x2000000, 0, "inv_drop_jewelry.anm"}, {0x800000 | 0x200000, 0, "inv_drop_mdwp_wd.anm"}, {0x200000 | 0x40000, 3, "inv_drop_chain.anm"},
        {0x400000 | 0x40000, 3, "inv_drop_sm_shield.anm"}, {0x40000 | 0x80000, 0, "inv_drop_mdwp_mtl.anm"}, {0x80000 | 0x1000000, 0, "inv_drop_cloth.anm"}};
    local = 0;
    for (const auto& n : names) {
        setup(1, n.type, n.flags); played.clear(); unequipItem__FiPQ24item11description(1, slot);
        if (played != "" && played != n.name) return 15;
        for (const std::string& c : calls) if (c.rfind("lump", 0) == 0) return 16;      // no sound for a remote player
    }
    local = 1;
    for (const auto& n : names) {
        setup(1, n.type, n.flags); played.clear(); unequipItem__FiPQ24item11description(1, slot);
        if (played != n.name) return 17;
    }
    return 0;
}
