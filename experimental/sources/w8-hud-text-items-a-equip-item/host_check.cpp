// Host contract for equipItem__FiPQ24item11descriptioni. Not an EE plan input.
// Callees are recording stubs; expectations encode the observed branch structure.
#define NORRATH_HOST_CONTRACT 1
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
static unsigned char arena[1024] __attribute__((aligned(16)));
unsigned char* norrath_host_base = reinterpret_cast<unsigned char*>(reinterpret_cast<std::uintptr_t>(arena) & ~std::uintptr_t(0xffffffffu));
#include "equip_item.cpp"
extern "C" {
unsigned char player[2 * 23984 + 64] __attribute__((aligned(16)));
unsigned char inventory[0x1d0];
unsigned char norrath_inventory_anim_state_view[4 * 264];
}
static std::vector<std::string> trace;
static int wieldStatus; static int localPlayer = 1; static long long featRank;
static unsigned short textA[2], textB[2];
static std::string S(const char* s) { return s; }
extern "C" {
int inventoryCheckWieldingStatus__FiPQ24item11description(int i, void*) { trace.push_back("check:" + std::to_string(i)); return wieldStatus; }
int netPlayerIsLocal__Fi(int i) { trace.push_back("local:" + std::to_string(i)); return localPlayer; }
void* lumpFindResource__FPcT0(char* l, char* n) { trace.push_back(S("lump:") + l + "/" + n); return arena + 1000; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) {
    trace.push_back(std::string("sound:") + (h == arena + 1000 ? "found" : "?") + (o ? ":obj" : ":null") + (v == 1.0f ? ":1" : ":?"));
}
unsigned short* textGetString__FPc(char* k) { trace.push_back(S("text:") + k); return std::strcmp(k, "ElfNoBigTitle") == 0 ? textA : textB; }
void multiHudAddHelpMessage__FiPUsT1i(int i, unsigned short* a, unsigned short* b, int w) {
    trace.push_back("help:" + std::to_string(i) + (a == textA ? ":T" : ":?") + (b == textB ? ":R" : ":?") + ":" + std::to_string(w));
}
void scriptStart__FPc(char* n) { trace.push_back(S("script:") + n); }
void scriptSetGameState__FPci(char* n, int v) { trace.push_back(S("state:") + n + "=" + std::to_string(v)); }
long long playerGetFeatRank__Fix(int i, long long f) { trace.push_back("feat:" + std::to_string(i) + ":" + std::to_string(f)); return featRank; }
void resetPlayerAnimations__Fi(int i) { trace.push_back("reset:" + std::to_string(i)); }
void inventorySetAnim__FiP14AnimationStatei(int i, void* r, int m) {
    trace.push_back("anim:" + std::to_string(i) + ":" + std::to_string((static_cast<unsigned char*>(r) - norrath_inventory_anim_state_view) / 264) + ":" + std::to_string(m));
}
void playerRecomputeAbilityModifiers__Fv() { trace.push_back("recompute"); }
}
// sprintf stub from the host libc is used through the real declaration.
static unsigned W(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
// arena: items at 0x00.. (32 bytes each), descriptions at 0x200.. (72 bytes each).
static unsigned char* itemP(int n) { return arena + n * 32; }
static unsigned char* descP(int n) { return arena + 0x200 + n * 72; }
static void makeItem(int n, int type, int hand, int sub) {
    std::memset(itemP(n), 0, 32);
    itemP(n)[28] = static_cast<unsigned char>(type); itemP(n)[29] = static_cast<unsigned char>(hand); itemP(n)[30] = static_cast<unsigned char>(sub);
}
static void makeDesc(int n, int item, unsigned long long flags) {
    std::memset(descP(n), 0, 72);
    unsigned w = item < 0 ? 0 : W(itemP(item)); std::memcpy(descP(n), &w, 4);
    std::memcpy(descP(n) + 8, &flags, 8);
}
static unsigned char* Pof(int i) { return player + i * 23984; }
static unsigned rd(int idx, int off) { unsigned v; std::memcpy(&v, Pof(idx) + off, 4); return v; }
static void wr(int idx, int off, unsigned v) { std::memcpy(Pof(idx) + off, &v, 4); }
static void reset() {
    std::memset(arena, 0, sizeof arena); std::memset(player, 0, sizeof player); std::memset(inventory, 0, sizeof inventory);
    trace.clear(); wieldStatus = 0; localPlayer = 1; featRank = 0;
}
#define EQ(a, b) do { if ((a) != (b)) { std::fprintf(stderr, "line %d\n", __LINE__); return __LINE__; } } while (0)
#define TRACE(...) do { std::vector<std::string> want{__VA_ARGS__}; if (trace != want) { std::fprintf(stderr, "line %d trace:", __LINE__); for (auto& t : trace) std::fprintf(stderr, " %s", t.c_str()); std::fprintf(stderr, "\n want:"); for (auto& t : want) std::fprintf(stderr, " %s", t.c_str()); std::fprintf(stderr, "\n"); return __LINE__; } } while (0)

int main() {
    // Refusal paths.
    reset(); makeItem(0, 1, 0, 1); makeDesc(0, 0, 0); wieldStatus = 3;
    equipItem__FiPQ24item11descriptioni(1, descP(0), 0);
    TRACE("check:1", "local:1", "lump:fx.lmp/game_butn_cantdo.anm", "sound:found:null:1", "text:ElfNoBigTitle", "text:NoWieldLowStr", "help:1:T:R:0");
    reset(); makeItem(0, 1, 0, 1); makeDesc(0, 0, 0); wieldStatus = 8; localPlayer = 0;
    wr(0, 0, 0); std::memcpy(inventory + 56, "\x05\0\0\0", 4); std::memcpy(inventory + 172, "\x00\0\0\0", 4);
    equipItem__FiPQ24item11descriptioni(0, descP(0), 0);   // inventory+56 != 0, +172 == 0 -> which = 1
    TRACE("check:0", "local:0", "text:ElfNoBigTitle", "text:NoWieldLowSlashing", "help:0:T:R:1");
    reset(); makeItem(0, 1, 0, 1); makeDesc(0, 0, 0); wieldStatus = 1; localPlayer = 0;
    std::memcpy(inventory + 56, "\x00\0\0\0", 4);
    equipItem__FiPQ24item11descriptioni(0, descP(0), 0);   // +56 == index -> which = 0
    TRACE("check:0", "local:0", "text:ElfNoBigTitle", "text:NoWieldWrongClass", "help:0:T:R:0");
    // Empty item word and type 15 do nothing at all.
    reset(); makeDesc(0, -1, 0); equipItem__FiPQ24item11descriptioni(0, descP(0), 0); TRACE();
    reset(); makeItem(0, 15, 0, 0); makeDesc(0, 0, 0); equipItem__FiPQ24item11descriptioni(0, descP(0), 0); TRACE();
    // One-handed weapon (type 1, hand 0, sub 1), loud. Flags 0x40000 + small weapon.
    reset(); makeItem(0, 1, 0, 1); makeDesc(0, 0, 0x40000);
    equipItem__FiPQ24item11descriptioni(1, descP(0), 0);
    TRACE("check:1", "script:*weaponEquipNotify", "reset:1", "anim:1:1:0", "local:1", "lump:fx.lmp/inv_equip_lgwp_mtl.anm", "sound:found:null:1", "anim:1:1:0", "recompute");
    EQ(rd(1, 23572) & 0xff, 0u); EQ(Pof(1)[23582], 1);
    EQ(rd(1, 21960), W(descP(0))); EQ(rd(1, 21900), W(descP(0)));
    // Bow (sub 4), quiet: slot 1, state flag, nothing played.
    reset(); makeItem(0, 1, 0, 4); makeDesc(0, 0, 0);
    equipItem__FiPQ24item11descriptioni(0, descP(0), 1);
    TRACE("check:0", "script:*bowEquipNotify", "state:bowEquipped=1", "reset:0", "anim:0:0:0", "anim:0:0:0", "recompute");
    EQ(Pof(0)[23572], 1); EQ(rd(0, 21964), W(descP(0))); EQ(rd(0, 21960), 0u); EQ(Pof(0)[23582], 4);
    // Type 8 (hand 0): no slot, no reset/animation, but equipped is recorded.
    reset(); makeItem(0, 8, 0, 2); makeDesc(0, 0, 0x1000000);
    equipItem__FiPQ24item11descriptioni(0, descP(0), 0);
    TRACE("check:0", "local:0", "lump:fx.lmp/inv_equip_potion.anm", "sound:found:null:1", "anim:0:0:0", "recompute");
    EQ(rd(0, 21896 + 32), W(descP(0)));
    // Type 2 clears the first three equipped words, plus extras for non-bow subs.
    reset(); makeItem(0, 2, 0, 3); makeDesc(0, 0, 0x100000);
    for (int off : {21896, 21900, 21904, 21908, 21968}) wr(0, off, 0x77);
    equipItem__FiPQ24item11descriptioni(0, descP(0), 1);
    EQ(rd(0, 21896), 0u); EQ(rd(0, 21900), 0u); EQ(rd(0, 21908), 0u); EQ(rd(0, 21968), 0u); EQ(rd(0, 21904), W(descP(0)));
    reset(); makeItem(0, 2, 0, 4); makeDesc(0, 0, 0);
    for (int off : {21908, 21968}) wr(0, off, 0x77);
    equipItem__FiPQ24item11descriptioni(0, descP(0), 1);
    EQ(rd(0, 21908), 0x77u); EQ(rd(0, 21968), 0x77u);
    // Already equipped: only the unconditional tail runs after the status/slot work.
    reset(); makeItem(0, 5, 1, 0); makeDesc(0, 0, 0); wr(0, 21896 + 20, W(descP(0)));
    equipItem__FiPQ24item11descriptioni(0, descP(0), 1);
    TRACE("check:0", "anim:0:0:0", "recompute");
    // Hand 2, type 5: retargets to the type-6 word when type-5 is taken and type-6 free.
    reset(); makeItem(0, 5, 2, 0); makeDesc(0, 0, 0); wr(0, 21916, 0x99);
    equipItem__FiPQ24item11descriptioni(0, descP(0), 1);
    EQ(rd(0, 21920), W(descP(0))); EQ(rd(0, 21916), 0x99u);
    reset(); makeItem(0, 5, 2, 0); makeDesc(0, 0, 0); wr(0, 21916, 0x99); wr(0, 21920, 0x55);
    equipItem__FiPQ24item11descriptioni(0, descP(0), 1);
    EQ(rd(0, 21916), W(descP(0))); EQ(rd(0, 21920), 0x55u);
    reset(); makeItem(0, 5, 2, 0); makeDesc(0, 0, 0);
    equipItem__FiPQ24item11descriptioni(0, descP(0), 1);
    EQ(rd(0, 21916), W(descP(0)));
    // Hand 1, type 3 (shield): second slot holds a hand-0 item => cleared, B[stored] cleared, byte 5.
    reset(); makeItem(0, 3, 1, 0); makeItem(1, 1, 0, 1); makeDesc(0, 0, 0); makeDesc(1, 1, 0);
    wr(0, 21904, W(descP(1)));
    equipItem__FiPQ24item11descriptioni(0, descP(0), 1);
    // type 3: slot becomes 0 and B[0] (zero here) is unequipped by recursion only if set.
    EQ(rd(0, 21904), 0u); EQ(Pof(0)[23582], 5); EQ(rd(0, 21908), W(descP(0)));
    TRACE("check:0", "reset:0", "anim:0:0:0", "anim:0:0:0", "recompute");
    // Shield with nothing in the second slot but P+21968 set: byte 0, 21968 and A[1] cleared.
    reset(); makeItem(0, 3, 1, 0); makeDesc(0, 0, 0); wr(0, 21968, 0x44); wr(0, 21900, 0x33);
    equipItem__FiPQ24item11descriptioni(0, descP(0), 1);
    EQ(rd(0, 21968), 0u); EQ(rd(0, 21900), 0u); EQ(Pof(0)[23582], 0);
    TRACE("check:0", "reset:0", "anim:0:0:0", "anim:0:0:0", "recompute");
    // Shield with nothing at all: no reset, only the store.
    reset(); makeItem(0, 3, 1, 0); makeDesc(0, 0, 0);
    equipItem__FiPQ24item11descriptioni(0, descP(0), 1);
    TRACE("check:0", "anim:0:0:0", "recompute");
    // Shield: a held B[0] description is unequipped first (recursion with quiet 0).
    reset(); makeItem(0, 3, 1, 0); makeItem(1, 4, 1, 0); makeDesc(0, 0, 0); makeDesc(1, 1, 0);
    wr(0, 21960, W(descP(1)));
    equipItem__FiPQ24item11descriptioni(0, descP(0), 1);
    EQ(trace.size() > 4, true);
    EQ(trace[0], std::string("check:0")); EQ(trace[1], std::string("check:0"));
    // Main-hand type 0 with positive feat rank and a type-0 B[0]: combined handling.
    reset(); makeItem(0, 0, 0, 1); makeItem(1, 0, 0, 1); makeDesc(0, 0, 0); makeDesc(1, 1, 0);
    wr(0, 21960, W(descP(1))); featRank = 1;
    equipItem__FiPQ24item11descriptioni(0, descP(0), 1);
    EQ(rd(0, 21968), W(descP(0)));
    // Dual-wield path for type 0: with A[0] set after the clear, byte 7 and A[1] receive the item.
    reset(); makeItem(0, 0, 0, 1); makeDesc(0, 0, 0); wr(0, 21896, 0x66); wr(0, 21908, 0x22); featRank = 2;
    equipItem__FiPQ24item11descriptioni(0, descP(0), 1);
    EQ(Pof(0)[23582], 7); EQ(rd(0, 21908), 0u); EQ(rd(0, 21900), W(descP(0))); EQ(rd(0, 21896), 0x66u);
    // Type 0 where the second word holds a hand-0 item: both words are cleared.
    reset(); makeItem(0, 0, 0, 1); makeItem(1, 7, 0, 1); makeDesc(0, 0, 0); makeDesc(1, 1, 0);
    wr(0, 21904, W(descP(1))); wr(0, 21896, 0x66);
    equipItem__FiPQ24item11descriptioni(0, descP(0), 1);
    EQ(rd(0, 21904), 0u); EQ(rd(0, 21896), W(descP(0)));
    // Sound names.
    struct Case { unsigned long long flags; int type; int sub; const char* name; };
    const Case cases[] = {
        {0x800000, 1, 1, "inv_equip_lgwp_wd.anm"}, {0x800000, 1, 4, "inv_equip_lgwp_wd.anm"},
        {0x800000, 1, 5, "inv_equip_mdwp_wd.anm"}, {0x800000 | 0x20000000, 1, 5, "inv_equip_smwp_wd.anm"},
        {0x200000, 1, 1, "inv_equip_chain.anm"}, {0, 3, 0, "inv_equip_sm_shield.anm"},
        {0x400000, 1, 1, "inv_equip_plate.anm"}, {0x40000, 1, 2, "inv_equip_lgwp_mtl.anm"},
        {0x40000, 1, 4, "inv_equip_mdwp_mtl.anm"}, {0x40000 | 0x20000000, 1, 4, "inv_equip_smwp_mtl.anm"},
        {0x100000, 1, 1, "inv_equip_leather.anm"}, {0x80000, 1, 1, "inv_equip_cloth.anm"},
        {0x1000000, 1, 1, "inv_equip_potion.anm"}, {0x4000000, 1, 1, "inv_equip_jewelry.anm"}, {0, 1, 1, "inv_equip_jewelry.anm"},
        {0x200000 | 0x400000, 1, 1, "inv_equip_chain.anm"}, {0x400000 | 0x100000, 1, 1, "inv_equip_plate.anm"}};
    for (const Case& c : cases) {
        reset(); makeItem(0, c.type, 1, c.sub); makeDesc(0, 0, c.flags);
        wr(0, 21896 + c.type * 4, W(descP(0)));   // already equipped: only the sound remains
        equipItem__FiPQ24item11descriptioni(0, descP(0), 0);
        TRACE("check:0", "local:0", std::string("lump:fx.lmp/") + c.name, "sound:found:null:1", "anim:0:0:0", "recompute");
    }
    return 0;
}
