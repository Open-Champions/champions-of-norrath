// Host contract for msg_run__15FrostStormSpell; not an EE input. 32-bit words are offsets from a host base and the callees log. The
// authored function was also compared with the original in a private R5900 run (random rolls with rejected draws, scale values
// around 1.0, reaches around the loop bounds, frame multiples of 10 and 30, list contents, owner node and slot results, allocation
// results and end frames; every original word covered).
#define W12_SPELL_C_HOST 1
#include "msg_run.cpp"
#include <cmath>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w12_spell_c_host_base;
static std::string calls;
static void* entry_object;
static unsigned yes_for;
unsigned w12_spell_c_host_call(unsigned fn, void* object, int count, unsigned long a1, unsigned long) {
    calls += "entry" + std::to_string(fn) + "(" + std::to_string(count) + "," + std::to_string(a1) + ");";
    entry_object = object;
    return fn == 0x70 && a1 == yes_for ? 1 : 0;
}
int w14b_host_call3(unsigned, void*, int, int, void*) { return 0; }
static std::vector<int> rolls;
static size_t rolls_used;
struct Added { Point3 where, velocity; void* def; float age; };
static std::vector<Added> added;
static float ground;
static bool alloc_ok;
static unsigned char blocks[8][128] __attribute__((aligned(16)));
static int blocks_used;
static std::vector<void*> grid, run, constructed;
static Point3 box_low, box_high;
static std::vector<void*> found;
struct Attack { void* target; void* source; float a, b; int c; short slot[4]; };
static std::vector<Attack> attacks;
static float light_updates;
extern "C" {
int frameNm;
void* world;
unsigned char norrath_w13_spell_msg_a_view_7d6a10[80];
unsigned char norrath_w13_spell_msg_a_view_7d6a60[80];
unsigned char norrath_w13_spell_msg_a_view_7d6b00[80];
unsigned char vt_FrostStormIceShard[8] __asm__("_vt$18FrostStormIceShard");
int eRand__10GameObject(void*) { return rolls[rolls_used++ % rolls.size()]; }
float icos__Fi(int) { return 1.0f; }
float isin__Fi(int) { return 0.5f; }
void lightUpdatePosition__FP5Light(void*) { calls += "light;"; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float f) { added.push_back({*p, *v, d, f}); }
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; return alloc_ok ? blocks[blocks_used++] : nullptr; }
void __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(void* self, Point3, void* map, void* names) { calls += "ctor;"; constructed.push_back(self); (void)map; (void)names; }
void objectUpdateInGrid__FP10GameObject(void* o) { calls += "grid;"; grid.push_back(o); }
void objectAddToRunList__FP10GameObject(void* o) { calls += "run;"; run.push_back(o); }
float worldFindPointHeight__FP12_worldHeaderG6Point3(void*, Point3) { calls += "ground;"; return ground; }
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3* low, Point3* high, void** list, int) {
    calls += "box;"; box_low = *low; box_high = *high;
    for (size_t i = 0; i < found.size(); ++i) list[i] = found[i];
    list[found.size()] = nullptr;
}
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void* b, float x, float y, int c, int, int, int, void* d, bool) {
    calls += "attack;";
    Attack at{a, b, x, y, c, {}};
    std::memcpy(at.slot, static_cast<unsigned char*>(d) + 36, 8);
    attacks.push_back(at);
}
}
static unsigned char arena[0x3000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static float getf(unsigned off) { float v; std::memcpy(&v, arena + off, 4); return v; }
static unsigned get32(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
static float blockf(int b, int off) { float v; std::memcpy(&v, blocks[b] + off, 4); return v; }
static unsigned blockw(int b, int off) { unsigned v; std::memcpy(&v, blocks[b] + off, 4); return v; }
enum { O = 0x100, NODE = 0x700, OWN = 0x780, OWNT = 0x7c0, VT = 0x800, E1 = 0x900, E2 = 0x940 };
static void reset(int frame, int start, int end, float k, float scale) {
    w12_spell_c_host_base = arena;
    std::memset(arena, 0, sizeof arena); std::memset(blocks, 0, sizeof blocks);
    calls.clear(); added.clear(); attacks.clear(); grid.clear(); run.clear(); constructed.clear(); found.clear();
    rolls = {49152}; rolls_used = 0; entry_object = nullptr; yes_for = 0xffffffffu; ground = 0.0f; alloc_ok = true; blocks_used = 0;
    frameNm = frame;
    put32(O + 80, static_cast<unsigned>(start)); put32(O + 76, static_cast<unsigned>(end)); putf(O + 84, k); putf(O + 88, scale);
    putf(O + 16, 1); putf(O + 20, 2); putf(O + 24, 3); putf(O + 96, 0.5f); putf(O + 100, 0.25f); putf(O + 104, 1.0f);
    putf(O + 140, 4.0f);
    put32(O + 72, VT); put32(VT + 8 + 4, 0x55); put32(O + 92, 25);
}
int main() {
    // not started yet: nothing at all
    reset(10, 10, 1000, 100.0f, 0.9f);
    msg_run__15FrostStormSpell(arena + O);
    if (!calls.empty() || getf(O + 16) != 1.0f) return 1;
    // shrinking (scale below 1): moves, scales K, light singles; no particles
    reset(11, 10, 1000, 100.0f, 0.5f);
    msg_run__15FrostStormSpell(arena + O);
    if (calls != "light;" || getf(O + 16) != 1.5f || getf(O + 20) != 2.25f || getf(O + 24) != 4.0f || getf(O + 84) != 50.0f) return 2;
    if (getf(O + 160) != 1.5f || getf(O + 164) != 2.25f || getf(O + 168) != 4.0f) return 3;
    if (getf(O + 140) != 2.0f || getf(O + 148) != 2.0f || getf(O + 144) != 0.25f || getf(O + 152) != 0.25f) return 4;
    // steady (scale exactly 1.0) at K = 100 on a frame that is a multiple of 10 but not 30: 17 + 17 particles, 4 ice shards, 34 ground samples
    reset(40, 10, 1000, 100.0f, 1.0f); ground = 0.0f;
    msg_run__15FrostStormSpell(arena + O);
    int d1 = 0, d2 = 0, d4 = 0;
    for (auto& a : added) {
        if (a.def == norrath_w13_spell_msg_a_view_7d6a10) ++d1;
        if (a.def == norrath_w13_spell_msg_a_view_7d6a60) ++d2;
        if (a.def == norrath_w13_spell_msg_a_view_7d6b00) ++d4;
    }
    if (d1 != 17 || d2 != 17 || d4 != 34 || constructed.size() != 4) return 5;
    // first definition-1 particle: x = unit.x * (K * 1.0000119) + P.x; z = H + P.z with H = 0.75 * 12 + 120; velocity z = 0 - |unit * 7 * CD|
    if (std::fabs(added[0].where.x - (0.5f * 100.0f * 1.0000119f + 1.5f)) > 1e-2 || std::fabs(added[0].where.z - (129.0f + 4.0f)) > 1e-3) return 6;
    if (added[0].velocity.x != 3.5f || added[0].velocity.z >= 0.0f || added[0].age != 0.0f) return 7;
    // shard objects: vtable word, flag bits, position, time, velocity, size
    if (blockw(0, 72) != w12c::to_word(vt_FrostStormIceShard) || (blockw(0, 12) & 0x421d) != 0x421d || blockw(0, 96) != 160 || blockf(0, 80) != 1.0f || blockf(0, 28) != 12.0f || blockw(0, 76) != 0) return 8;
    if (std::fabs(blockf(0, 16) - (0.5f * 100.0f * 1.0000019f + 1.5f)) > 1e-1 || blockf(0, 96 - 96 + 84 + 8) >= 0.0f) return 9;
    if (grid.size() != 4 || run.size() != 4) return 10;
    // a ground far below the object skips the ground particles (the test is P.z - 148 <= height + 5)
    reset(40, 10, 1000, 100.0f, 1.0f); ground = -400.0f;
    msg_run__15FrostStormSpell(arena + O);
    int low4 = 0;
    for (auto& a : added) if (a.def == norrath_w13_spell_msg_a_view_7d6b00) ++low4;
    if (low4 != 0) return 11;
    reset(40, 10, 1000, 100.0f, 1.0f); ground = 0.0f; putf(O + 104, 600.0f);
    msg_run__15FrostStormSpell(arena + O);
    low4 = 0;
    for (auto& a : added) if (a.def == norrath_w13_spell_msg_a_view_7d6b00) ++low4;
    if (low4 != 0) return 12;
    // allocation failure builds no shards
    reset(40, 10, 1000, 100.0f, 1.0f); alloc_ok = false;
    msg_run__15FrostStormSpell(arena + O);
    if (!constructed.empty()) return 13;
    // frame 30: box of half-size K, an owner node gates the objects with slot 160 and sets the attacker
    reset(30, 10, 1000, 100.0f, 0.5f);
    found = {arena + E1, arena + E2}; put32(O + 108, NODE); put32(NODE + 12, OWN); put32(OWN + 72, OWNT); put32(OWNT + 160 + 4, 0x70);
    yes_for = E2;
    msg_run__15FrostStormSpell(arena + O);
    if (box_low.x != 1.5f - 50.0f || box_high.z != 4.0f + 50.0f) return 14;
    if (attacks.size() != 1 || attacks[0].target != arena + E2 || attacks[0].source != arena + OWN || attacks[0].a != 12.5f || attacks[0].b != 12.5f || attacks[0].c != 100000) return 15;
    if (attacks[0].slot[0] != 22 || attacks[0].slot[1] != 12 || attacks[0].slot[2] != 12) return 16;
    // without an owner node every object is attacked and the attacker is null
    reset(30, 10, 1000, 100.0f, 0.5f);
    found = {arena + E1, arena + E2};
    msg_run__15FrostStormSpell(arena + O);
    if (attacks.size() != 2 || attacks[0].source != nullptr) return 17;
    // life over: the scale becomes 0.95 and, below a reach of 12, the entry at offset 8 runs with 3
    reset(50, 10, 40, 10.0f, 0.5f);
    msg_run__15FrostStormSpell(arena + O);
    if (std::fabs(getf(O + 88) - 0.95f) > 1e-6 || calls.find("entry85(1,3);") == std::string::npos) return 18;
    reset(50, 10, 40, 100.0f, 0.5f);
    msg_run__15FrostStormSpell(arena + O);
    if (std::fabs(getf(O + 88) - 0.95f) > 1e-6 || calls.find("entry85") != std::string::npos) return 19;
    (void)get32; (void)light_updates;
    return 0;
}
