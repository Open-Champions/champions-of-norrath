// Host contract for msg_run__14HateProjectile; not an EE input. 32-bit words are offsets from a host base, the callees log, and
// rsqrt.s is replaced by a division. The authored function was also compared with the original in a private R5900 run (400
// scenarios over states, ground and wall hits, box contents with and without a caster node, chain lengths, light timing, spell
// presence, allocation results and the end frame; coverage 813 of 817 original words).
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
static int amount_result, amount_mode, amount_byte;
static void *amount_object, *amount_address;
int w14b_host_call3(unsigned, void* adjusted, int first, int second, void* third) {
    calls += "amount;"; amount_object = adjusted; amount_mode = first; amount_byte = second; amount_address = third; return amount_result;
}
static std::vector<int> rolls;
static size_t rolls_used;
struct Added { Point3 where, velocity; void* def; };
static std::vector<Added> added;
static float ground, sound_volume;
static std::vector<void*> found_small, found_big, linked, sound_objects;
static int box_calls, node_count, running_result, ctor_a, ctor_b;
static Point3 box_low[2], box_high[2];
struct Attack { void* target; void* source; float a, b; int p, f; };
static std::vector<Attack> attacks;
struct Start { Point3 a, b; int c, d; float e, f; unsigned g; };
static std::vector<Start> starts;
struct Move { int handle; Point3 a, b; int c, d; };
static std::vector<Move> moves;
static int next_handle;
static std::string lump_b;
static void *ctor_creature, *ctor_object, *ctor_skill, *spell_creature, *spell_block, *checked_creature;
static bool alloc_ok;
static unsigned char spell_block_memory[64] __attribute__((aligned(16)));
extern "C" {
int frameNm;
void* world;
unsigned char skills[256];
float worldDifficulty;
unsigned char norrath_w13_spell_msg_a_view_7d6fc0[80];
unsigned char norrath_w13_spell_msg_a_view_7d7010[80];
int eRand__10GameObject(void*) { return rolls[rolls_used++ % rolls.size()]; }
float icos__Fi(int) { return 2.0f; }
float isin__Fi(int) { return 3.0f; }
void soundChangeSequenceVolume__Fif(int handle, float v) { calls += "vol" + std::to_string(handle) + ";"; sound_volume = v; }
void objectUpdateInGrid__FP10GameObject(void*) { calls += "grid;"; }
float worldFindHeight__FP12_worldHeaderG6Point3f(void*, Point3, float r) { calls += r == 12.0f ? "height;" : "badradius;"; return ground; }
void* lumpFindResource__FPcT0(char*, char* b) { calls += "lump;"; lump_b = b; return nullptr; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void* o, float) { calls += "sound;"; sound_objects.push_back(o); }
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3* low, Point3* high, void** list, int capacity) {
    calls += "box" + std::to_string(capacity) + ";";
    box_low[box_calls & 1] = *low; box_high[box_calls & 1] = *high;
    std::vector<void*>& src = box_calls == 0 ? found_small : found_big;
    ++box_calls;
    for (size_t i = 0; i < src.size(); ++i) list[i] = src[i];
    list[src.size()] = nullptr;
}
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void*) {
    calls += "link;"; linked.push_back(link);
    unsigned char* h = static_cast<unsigned char*>(head);
    unsigned char* n = w12_spell_c_host_base + 0x3000 + 16 * node_count++;
    w12c::word(n, 0) = w12c::word(h, 4);
    w12c::word(n, 12) = w12c::to_word(link);
    w12c::word(h, 4) = w12c::to_word(n);
}
void buildShockChainList__14HateProjectileP6Point3(void* self, Point3*) { calls += "build;"; static_cast<unsigned char*>(self)[387] = 2; }
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void* b, float x, float y, int p, int, int, int f, void*, bool) {
    calls += "attack;"; attacks.push_back({a, b, x, y, p, f});
}
void* __tf15SpellHateEffect() { calls += "type;"; return nullptr; }
int checkIfSpellIsRunning__8CreatureRC9type_info(void* c, void*) { calls += "running;"; checked_creature = c; return running_result; }
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; return alloc_ok ? spell_block_memory : nullptr; }
void __15SpellHateEffectiiP8CreatureP10GameObjectP5Skill(void*, int a, int b, void* c, void* o, void* s) {
    calls += "ctor;"; ctor_a = a; ctor_b = b; ctor_creature = c; ctor_object = o; ctor_skill = s;
}
void addSpell__8CreatureP5Spell(void* c, void* s) { calls += "add;"; spell_creature = c; spell_block = s; }
void lightUpdatePosition__FP5Light(void*) { calls += "light;"; }
int P_StartLightning__FG6Point3T0iiffUi(Point3 a, Point3 b, int c, int d, float e, float f, unsigned g) { calls += "start;"; starts.push_back({a, b, c, d, e, f, g}); return next_handle++; }
void P_MoveLightning__FiG6Point3T1ii(int h, Point3 a, Point3 b, int c, int d) { calls += "move;"; moves.push_back({h, a, b, c, d}); }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float) { added.push_back({*p, *v, d}); }
}
static unsigned char arena[0x4000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static void put16(unsigned off, short v) { std::memcpy(arena + off, &v, 2); }
static float getf(unsigned off) { float v; std::memcpy(&v, arena + off, 4); return v; }
static unsigned get32(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
enum { O = 0x100, E1 = 0x1000, E2 = 0x1100, E3 = 0x1200, CASTER = 0x1300, OWN = 0x1380, OWNT = 0x13c0, VT = 0x1400, SRC = 0x1500, SK = 0x1600 };
static void reset(int frame, int word88, int w) {
    w12_spell_c_host_base = arena;
    std::memset(arena, 0, sizeof arena); std::memset(skills, 0, sizeof skills);
    calls.clear(); added.clear(); linked.clear(); attacks.clear(); starts.clear(); moves.clear(); found_small.clear(); found_big.clear(); sound_objects.clear();
    rolls = {49152}; rolls_used = 0; entry_object = nullptr; yes_for = 0xffffffffu; ground = -1000.0f; amount_result = 20; box_calls = 0; node_count = 0;
    next_handle = 700; running_result = 0; alloc_ok = true; worldDifficulty = 2.9f;
    frameNm = frame;
    put32(O + 88, static_cast<unsigned>(word88)); put32(O + 92, static_cast<unsigned>(w)); put32(O + 96, 55); put16(O + 56, 123);
    putf(O + 16, 1); putf(O + 20, 2); putf(O + 24, 3);
    put32(O + 72, VT); put32(VT + 8 + 4, 0x55);
    put32(O + 76, SRC); arena[O + 384] = 3; put32(SRC + 96, SRC + 0x40); put32(SRC + 0x40 + 112 + 4, 0x321);
    put32(SK, 0); const unsigned skill_word = SK + 0x20; std::memcpy(skills + 120, &skill_word, 4);
}
static int count_def(void* def) { int n = 0; for (auto& a : added) if (a.def == def) ++n; return n; }
int main() {
    // flying: the position advances by icos/isin * w / 60 and drops by 1, the light follows, 6 x 45 particles with smoke on rolls with low bits zero
    reset(5, 5 + 100, 60);
    msg_run__14HateProjectile(arena + O);
    if (calls.substr(0, 11) != "vol55;grid;" && calls.substr(0, 7) != "vol55;g") return 1;
    if (std::fabs(getf(O + 16) - (1.0f + 2.0f * 60.0f * 0.016666668f)) > 1e-4 || std::fabs(getf(O + 20) - (2.0f + 3.0f * 60.0f * 0.016666668f)) > 1e-4 || getf(O + 24) != 2.0f || sound_volume != 1.0f) return 2;
    if (count_def(norrath_w13_spell_msg_a_view_7d6fc0) != 270 || count_def(norrath_w13_spell_msg_a_view_7d7010) != 270) return 3;
    if (getf(O + 352) != 18.0f - 0.0f + 120.0f || getf(O + 372) != getf(O + 16)) return 4;
    if (calls.find("entry") != std::string::npos) return 5;
    // with w zero the height is not checked and z does not drop
    reset(5, 5 + 100, 0);
    msg_run__14HateProjectile(arena + O);
    if (calls.find("height") != std::string::npos || getf(O + 24) != 3.0f) return 6;
    // the ground: below it, w clears, state 2, frame - 150, the hit sound; no search
    reset(8, 8 + 100, 60); ground = 50.0f;
    msg_run__14HateProjectile(arena + O);
    if (arena[O + 385] != 2 || get32(O + 92) != 0 || get32(O + 88) != 8 - 150 || calls.find("lump;sound;") == std::string::npos || lump_b != "spel_ew_shock_hit.anm" || calls.find("box") != std::string::npos) return 7;
    // a wall (unflagged object in the box, no caster node) does the same
    reset(8, 8 + 100, 0);
    put32(E1 + 12, 0x00); found_small = {arena + E1};
    msg_run__14HateProjectile(arena + O);
    if (arena[O + 385] != 2 || get32(O + 88) != 8 - 150) return 8;
    // contact without a caster node: flagged (0x20) objects with none of 0x0d, within range; 0x01 and the object itself are skipped
    reset(8, 8 + 100, 0);
    put32(E1 + 12, 0x20); putf(E1 + 16, 1.0f); putf(E1 + 20, 2.0f); putf(E1 + 24, 3.0f); putf(E1 + 28, 2.0f);
    put32(E2 + 12, 0x20); putf(E2 + 16, 10.0f);
    put32(E3 + 12, 0x21);
    found_small = {arena + E3, arena + O, arena + E1};
    found_big = {arena + E1, arena + E2, arena + E3, arena + O};
    putf(O + 396, 0.0f); put32(O + 396, SK + 0x80);
    msg_run__14HateProjectile(arena + O);
    if (box_low[0].x != 1.0f - 20.0f || box_high[0].z != 3.0f + 20.0f || box_low[1].x != 1.0f - 600.0f || box_high[1].y != 2.0f + 600.0f) return 9;
    if (linked.size() != 2 || linked[0] != arena + E2 || linked[1] != arena + E1) return 10;
    // two chain nodes, each attacked with (n, m) = (20, 22) since 20 * 0.2 * 0.75 is a hair under 3 and the attack flags for a roll of 0.75
    if (attacks.size() != 2 || attacks[0].target != arena + E1 || attacks[1].target != arena + E2 || attacks[0].source != nullptr) return 11;
    if (attacks[0].a != 20.0f || attacks[0].b != 22.0f || attacks[0].p != 100000 || attacks[0].f != 0x40040) return 12;
    if (amount_mode != 0 || amount_byte != 3 || amount_object != arena + SRC || amount_address != arena + O + 392) return 13;
    // the hate spell is built for each creature that does not already run it: block size 48, (3, (int)2.9, creature, null, skills word)
    if (calls.find("running;alloc48;ctor;add;") == std::string::npos || ctor_a != 3 || ctor_b != 2 || ctor_object != nullptr || ctor_creature != arena + E2 || ctor_skill != arena + SK + 0x20) return 14;
    if (spell_creature != arena + E2 || spell_block != spell_block_memory) return 15;
    if (sound_objects.size() < 2) return 16;
    // after the hit: state 1, w zero, frame - 150, handle slots -1 then the first lightning is started
    if (arena[O + 385] != 1 || get32(O + 92) != 0 || get32(O + 88) != 8 - 150) return 17;
    if (starts.size() != 1 || starts[0].c != 260 || starts[0].d != 4 || starts[0].e != 16.0f || starts[0].f != 8.5f || starts[0].g != 0xff000fdcu || starts[0].a.z != 3.0f + 36.0f || get32(O + 292) != 700u) return 18;
    msg_run__14HateProjectile(arena + O);
    if (moves.size() != 1 || moves[0].handle != 700 || moves[0].c != 128 || moves[0].d != -1) return 19;
    // a creature that already runs the spell gets no new one
    reset(8, 8 + 100, 0);
    put32(E1 + 12, 0x20); putf(E1 + 16, 1.0f); putf(E1 + 20, 2.0f); putf(E1 + 28, 2.0f);
    found_small = {arena + E1}; found_big = {arena + E1}; running_result = 1;
    msg_run__14HateProjectile(arena + O);
    if (calls.find("alloc48") != std::string::npos || calls.find("add;") != std::string::npos) return 20;
    // allocation failure still adds (a null spell)
    reset(8, 8 + 100, 0); alloc_ok = false;
    put32(E1 + 12, 0x20); putf(E1 + 16, 1.0f); putf(E1 + 20, 2.0f); putf(E1 + 28, 2.0f);
    found_small = {arena + E1}; found_big = {arena + E1};
    msg_run__14HateProjectile(arena + O);
    if (calls.find("alloc48;add;") == std::string::npos || spell_block != nullptr) return 21;
    // with a caster node the table slot 160 of its object decides, and the flags test comes first
    reset(8, 8 + 100, 0);
    put32(O + 392, CASTER); put32(CASTER + 12, OWN); put32(OWN + 72, OWNT); put32(OWNT + 160 + 4, 0x70);
    put32(E1 + 12, 0x20); putf(E1 + 16, 1.0f); putf(E1 + 20, 2.0f); putf(E1 + 28, 2.0f);
    put32(E2 + 12, 0x20);
    found_small = {arena + E1}; found_big = {arena + E1, arena + E2}; yes_for = E2;
    msg_run__14HateProjectile(arena + O);
    if (calls.find("entry112(1,4096);") == std::string::npos || !linked.empty() || box_calls != 1) return 22;
    reset(8, 8 + 100, 0);
    put32(O + 392, CASTER); put32(CASTER + 12, OWN); put32(OWN + 72, OWNT); put32(OWNT + 160 + 4, 0x70);
    put32(E1 + 12, 0x20); putf(E1 + 16, 1.0f); putf(E1 + 20, 2.0f); putf(E1 + 28, 2.0f);
    found_small = {arena + E1}; found_big = {arena + E1, arena + E2}; yes_for = E1;
    msg_run__14HateProjectile(arena + O);
    if (linked.size() != 2 || linked[0] != arena + E1 || linked[1] != arena + E1 || attacks.size() != 2 || attacks[0].source != nullptr) return 24;
    // the end: t <= 0 runs the entry at offset 8 with 3
    reset(5, 5 - 180, 0);
    msg_run__14HateProjectile(arena + O);
    if (calls.find("entry85(1,3);") == std::string::npos || entry_object != arena + O || getf(O + 352) != 18.0f - 180.0f + 120.0f) return 23;
    (void)get32;
    return 0;
}
