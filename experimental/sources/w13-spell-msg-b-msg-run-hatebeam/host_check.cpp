// Host contract for msg_run__8HateBeam; not an EE input. 32-bit words are offsets from a host base and the callees log. The
// authored function was also compared with the original in a private R5900 run (66 scenarios over frameNm, the end word, the owner
// word, creature lists with several accepted entries, existing and missing bolt handles, spell checks, allocation failure and
// roll lists; every original word covered but a delay-slot nop). The one deliberate difference, the z of a random end point (the
// image reads an uninitialised stack float there), was masked in that comparison.
#define W13_SPELL_MSG_B_HOST 1
#include "../w13-spell-msg-b-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w13_spell_msg_b_host_base;
static std::string calls;
static std::vector<unsigned> creatures;       // creatures offered by the list
static std::vector<bool> accepted;            // the W entry 160 answer per creature
static bool spell_running[2];
static int spell_checks;
static std::vector<int> rolls;
static unsigned roll_index;
unsigned w13_spell_msg_b_host_call(unsigned fn, void*, int, unsigned long a1, unsigned long) {
    if (fn == 0x700) {
        for (size_t i = 0; i < creatures.size(); ++i) if (creatures[i] == a1) return accepted[i] ? 1 : 0;
        return 0;
    }
    if (fn == 0x710) return 0x300;  // the model
    calls += "entry" + std::to_string(fn) + "(" + std::to_string(a1) + ");";
    return 0;
}
struct Bolt { bool start; int handle; Point3 from, to; };
static std::vector<Bolt> bolts;
static unsigned next_handle;
static int attach_index_seen[2];
static float light_width;
static std::vector<int> ctor_args;
extern "C" {
int frameNm;
void* world = reinterpret_cast<void*>(0x44);
float worldDifficulty = 2.5f;
unsigned skills[64];
unsigned eRand__10GameObject(void*) { return rolls[roll_index++ % rolls.size()]; }
float icos__Fi(int a) { (void)a; calls += "cos;"; return 0.5f; }
float isin__Fi(int a) { calls += "sin" + std::to_string(a) + ";"; return -0.75f; }
void objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(Point3, short, float a, float b, void* head, void* creature) {
    calls += "list;"; if (a != 200.0f || b != 120.0f || creature != nullptr) calls += "BADLIST;";
    // nodes live at 0xe00 + 0x40 * i; the head word is the offset of the first
    unsigned first = creatures.empty() ? 0u : 0xe00u;
    std::memcpy(head, &first, 4);
    for (size_t i = 0; i < creatures.size(); ++i) {
        unsigned next = i + 1 < creatures.size() ? static_cast<unsigned>(0xe00 + 0x40 * (i + 1)) : 0u;
        std::memcpy(w13_spell_msg_b_host_base + 0xe00 + 0x40 * i, &next, 4);
        std::memcpy(w13_spell_msg_b_host_base + 0xe00 + 0x40 * i + 12, &creatures[i], 4);
    }
}
void listDestroy__FP8ListHead(void*) { calls += "destroy;"; }
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void* model, Point3, short, void*, int index, Point3* first, Point3* second, float) {
    calls += "attach" + std::to_string(index) + ";"; (void)model; if (second != nullptr) calls += "BADSECOND;";
    first->x = index == 0 ? 1.0f : 5.0f; first->y = 2.0f; first->z = 3.0f;
}
void lightUpdatePosition__FP5Light(void*) { calls += "light;"; }
float worldFindHeight__FP12_worldHeaderG6Point3f(void*, Point3, float radius) { calls += "height" + std::to_string(static_cast<int>(radius)) + ";"; return 9.5f; }
int P_StartLightning__FG6Point3T0iiffUi(Point3* from, Point3* to, int a, int b, float c, float d, unsigned color) {
    if (a != 550 || b != 5 || color != 0xff000fdcu || c < 7.9f || d < 11.9f) calls += "BADSTART;";
    bolts.push_back({true, static_cast<int>(next_handle), *from, *to}); return static_cast<int>(next_handle++);
}
void P_MoveLightning__FiG6Point3T1ii(int handle, Point3* from, Point3* to, int a, int b) {
    if (a != 94 || b != -1) calls += "BADMOVE;";
    bolts.push_back({false, handle, *from, *to});
}
const void* __tf15SpellHateEffect() { return w13_spell_msg_b_host_base + 0x300; }
int checkIfSpellIsRunning__8CreatureRC9type_info(void*, const void*) { calls += "check;"; return spell_running[spell_checks++ % 2] ? 1 : 0; }
static unsigned char spell_block[64];
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; return spell_block; }
void SpellHateEffect_construct(void*, int a, int b, void* creature, void* object, void* skill) __asm__("__15SpellHateEffectiiP8CreatureP10GameObjectP5Skill");
void SpellHateEffect_construct(void*, int a, int b, void* creature, void* object, void* skill) {
    calls += "ctor;"; ctor_args = {a, b, static_cast<int>(static_cast<unsigned char*>(creature) - w13_spell_msg_b_host_base)};
    if (object != nullptr || skill != reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(skills[30]))) calls += "BADCTOR;";
}
void addSpell__8CreatureP5Spell(void*, void*) { calls += "add;"; }
}
#include "msg_run.cpp"

static unsigned char arena[0x4000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static float getf(unsigned off) { float v; std::memcpy(&v, arena + off, 4); return v; }
static unsigned get32(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
enum { O = 0x100, Q = 0x900, W = 0xa00, TW = 0xb00, OT = 0xc00, C0 = 0x1400, C1 = 0x1800 };
static void reset(int frame, int end) {
    w13_spell_msg_b_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    calls.clear(); bolts.clear(); creatures.clear(); accepted.clear(); next_handle = 0x70; spell_checks = 0; spell_running[0] = spell_running[1] = false;
    roll_index = 0; rolls = {32768}; frameNm = frame; skills[30] = 0x1234;
    put32(O + 172, end); put32(O + 76, Q); put32(Q + 12, W); put32(W + 72, TW); put32(TW + 160 + 4, 0x700); put32(TW + 128 + 4, 0x710);
    put32(O + 72, OT); put32(OT + 8 + 4, 0x900);
    putf(W + 16, 100); putf(W + 20, 200); putf(W + 24, 300);
    putf(C0 + 16, 10); putf(C0 + 20, 20); putf(C0 + 24, 30); putf(C0 + 32, 8);
    putf(C1 + 16, 11); putf(C1 + 20, 21); putf(C1 + 24, 31); putf(C1 + 32, 4);
}
int main() {
    unsigned char* const o = arena + O;
    reset(50, 20);  // past the end: entry 8 with 3
    msg_run__8HateBeam(o);
    if (calls != "entry2304(3);") { std::puts(calls.c_str()); return 1; }
    reset(5, 100); put32(O + 76, 0);
    msg_run__8HateBeam(o);
    if (calls != "entry2304(3);") return 2;
    // one accepted creature, no bolts yet: six bolts are started, three from each attachment point, towards the creature
    reset(5, 100); creatures = {C0}; accepted = {true};
    msg_run__8HateBeam(o);
    if (bolts.size() != 6) { std::puts(calls.c_str()); return 3; }
    for (int i = 0; i < 6; ++i) {
        if (!bolts[i].start || bolts[i].from.x != (i < 3 ? 1.0f : 5.0f) || bolts[i].to.x != 10.0f || bolts[i].to.y != 20.0f || bolts[i].to.z != 30.0f + 4.0f) return 10 + i;
        if (get32(O + 80 + 4 * i) != 0x70u + i) return 20 + i;
    }
    if (calls.find("check;alloc48;ctor;add;check;") == std::string::npos) { std::puts(calls.c_str()); return 4; }
    if (ctor_args[0] != 3 || ctor_args[1] != 2 || ctor_args[2] != C0) return 5;
    // the light: width = (int)(-0.75 * 6.0) + 120 = 116
    if (getf(O + 104 + 28) != 116.0f || getf(O + 104 + 36) != 116.0f || getf(O + 104 + 32) != 1.0f / (116.0f * 116.0f)) return 6;
    // two accepted creatures: both targets are the last one; existing handles are moved, not started
    reset(5, 100); creatures = {C0, C1}; accepted = {true, true};
    for (int i = 0; i < 6; ++i) put32(O + 80 + 4 * i, 0x40 + i);
    msg_run__8HateBeam(o);
    if (bolts.size() != 6 || bolts[0].start || bolts[0].handle != 0x40 || bolts[5].handle != 0x45 || bolts[0].to.x != 11.0f || bolts[0].to.z != 31.0f + 2.0f) return 7;
    // a refused creature leaves no target: random end points at W + (cos * 200 * f, sin * 200 * f), z from worldFindHeight
    reset(5, 100); creatures = {C0}; accepted = {false};
    msg_run__8HateBeam(o);
    if (bolts.size() != 6 || !bolts[0].start || bolts[0].to.z != 9.5f || calls.find("height16;") == std::string::npos) { std::puts(calls.c_str()); return 8; }
    // 0.5 * 200 * 0.5 = 50 and -0.75 * 200 * 0.5 = -75 around W
    if (bolts[0].to.x != 100.0f + 50.0f || bolts[0].to.y != 200.0f - 75.0f) return 9;
    if (calls.find("check") != std::string::npos) return 30;
    // a running hate effect on the target blocks the new one
    reset(5, 100); creatures = {C0}; accepted = {true}; spell_running[0] = true;
    msg_run__8HateBeam(o);
    {
        size_t blocks = 0;
        for (size_t at = calls.find("alloc48;"); at != std::string::npos; at = calls.find("alloc48;", at + 1)) ++blocks;
        if (blocks != 1) return 32;  // the target's first check says running, the second does not
    }
    if (calls.find("check;check;") == std::string::npos) return 31;
    return 0;
}
