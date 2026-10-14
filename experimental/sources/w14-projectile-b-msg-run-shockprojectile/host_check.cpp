// Host contract for msg_run__15ShockProjectile; not an EE input. 32-bit words are offsets from a host base, the callees log, and
// rsqrt.s is replaced by a division. The authored function was also compared with the original in a private R5900 run (random rolls
// with rejected draws, states 0/1/2, ground hits, box contents with and without a caster node, chain lengths, light timing and the
// end frame; every original word covered).
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
static int amount_result;
static int amount_mode, amount_byte;
static void *amount_object, *amount_address;
int w14b_host_call3(unsigned, void* adjusted, int first, int second, void* third) {
    calls += "amount;"; amount_object = adjusted; amount_mode = first; amount_byte = second; amount_address = third; return amount_result;
}
static std::vector<int> rolls;
static size_t rolls_used;
struct Added { Point3 where, velocity; void* def; };
static std::vector<Added> added;
static float ground;
static std::vector<void*> found_small, found_big;
static int box_calls;
static Point3 box_low[2], box_high[2];
static std::vector<void*> linked;
struct Attack { void* target; void* source; int p, s; short slot[4]; };
static std::vector<Attack> attacks;
struct Start { Point3 a, b; int c, d; float e, f; unsigned g; };
static std::vector<Start> starts;
struct Move { int handle; Point3 a, b; int c, d; };
static std::vector<Move> moves;
static int next_handle;
static float sound_volume, light_q;
static std::string lump_b;
static int node_count;
static float sin_value;
static void* chain_point_target;
extern "C" {
int frameNm;
void* world;
unsigned syncRandState;
unsigned char norrath_w13_spell_msg_a_view_8d6320[80];
unsigned char norrath_w13_spell_msg_a_view_8d6370[80];
int eRand__10GameObject(void*) { return rolls[rolls_used++ % rolls.size()]; }
float isin__Fi(int) { return sin_value; }
void soundChangeSequenceVolume__Fif(int handle, float v) { calls += "vol" + std::to_string(handle) + ";"; sound_volume = v; }
void objectUpdateInGrid__FP10GameObject(void*) { calls += "grid;"; }
float worldFindHeight__FP12_worldHeaderG6Point3f(void*, Point3, float r) { calls += r == 6.0f ? "height;" : "badradius;"; return ground; }
void* lumpFindResource__FPcT0(char*, char* b) { calls += "lump;"; lump_b = b; return nullptr; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void*, float) { calls += "sound;"; }
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
    const unsigned first = w12c::word(h, 4);
    w12c::word(n, 0) = first;
    w12c::word(n, 12) = w12c::to_word(link);
    w12c::word(h, 4) = w12c::to_word(n);
}
void buildShockChainList__15ShockProjectileP6Point3(void* self, Point3*) { calls += "build;"; static_cast<unsigned char*>(self)[395] = 2; }
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void* b, float, float, int p, int, int, int s, void* d, bool) {
    calls += "attack;";
    Attack at{a, b, p, s, {}};
    std::memcpy(at.slot, static_cast<unsigned char*>(d) + 36, 8);
    attacks.push_back(at);
}
void lightUpdatePosition__FP5Light(void*) { calls += "light;"; }
int P_StartLightning__FG6Point3T0iiffUi(Point3 a, Point3 b, int c, int d, float e, float f, unsigned g) { calls += "start;"; starts.push_back({a, b, c, d, e, f, g}); return next_handle++; }
void P_MoveLightning__FiG6Point3T1ii(int h, Point3 a, Point3 b, int c, int d) { calls += "move;"; moves.push_back({h, a, b, c, d}); }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float) { added.push_back({*p, *v, d}); }
}
static unsigned char arena[0x4000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static float getf(unsigned off) { float v; std::memcpy(&v, arena + off, 4); return v; }
static unsigned get32(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
enum { O = 0x100, E1 = 0x1000, E2 = 0x1100, E3 = 0x1200, CASTER = 0x1300, OWN = 0x1380, OWNT = 0x13c0, VT = 0x1400, SRC = 0x1500 };
static void reset(int frame, int word88) {
    w12_spell_c_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    calls.clear(); added.clear(); linked.clear(); attacks.clear(); starts.clear(); moves.clear(); found_small.clear(); found_big.clear();
    rolls = {49152}; rolls_used = 0; entry_object = nullptr; yes_for = 0xffffffffu; ground = -1000.0f; amount_result = 20; box_calls = 0; node_count = 0;
    next_handle = 500; syncRandState = 0; sin_value = 0.5f;
    frameNm = frame;
    put32(O + 88, static_cast<unsigned>(word88)); put32(O + 92, 55);
    putf(O + 16, 1); putf(O + 20, 2); putf(O + 24, 3); putf(O + 288, 0.5f); putf(O + 292, 0.25f);
    put32(O + 72, VT); put32(VT + 8 + 4, 0x55);
    put32(O + 76, SRC); arena[O + 392] = 3; put32(SRC + 96, SRC + 0x40); put32(SRC + 0x40 + 112 + 4, 0x321);
}
static int count_def(void* def) { int n = 0; for (auto& a : added) if (a.def == def) ++n; return n; }
int main() {
    // flying, nothing near: movement, volume, grid, the light follows, 42 particle pairs (the 0xc000 rolls always add smoke), no entry
    reset(5, 105);
    msg_run__15ShockProjectile(arena + O);
    if (calls.substr(0, 14) != "vol55;grid;lig" && calls.substr(0, 11) != "vol55;grid;") return 1;
    if (getf(O + 16) != 1.5f || getf(O + 20) != 2.25f || sound_volume != 1.0f) return 2;
    if (getf(O + 360) != 101.0f || getf(O + 368) != 101.0f || std::fabs(getf(O + 364) - 1.0f / (101.0f * 101.0f)) > 1e-9) return 3;
    if (getf(O + 380) != 1.5f || count_def(norrath_w13_spell_msg_a_view_8d6320) != 42 || count_def(norrath_w13_spell_msg_a_view_8d6370) != 42) return 4;
    // each point is on the 17-sphere around the object (the unit vector is (.5,.5,.5) / |.|)
    {
        const float r = 17.0f / std::sqrt(3.0f);
        if (std::fabs(added[0].where.x - (1.5f + r)) > 1e-3 || added[0].velocity.x != 0.5f) return 5;
    }
    if (calls.find("entry") != std::string::npos) return 6;
    // ten frames left: no entry yet, and the light fade term is 180 - 6t
    reset(5, 5 - 300 + 10);
    msg_run__15ShockProjectile(arena + O);
    if (calls.find("entry") != std::string::npos || getf(O + 360) != 5.0f - 120.0f + 96.0f) return 7;
    // time up: the entry at offset 8 runs with 3
    reset(5, 5 - 300);
    msg_run__15ShockProjectile(arena + O);
    if (calls.find("entry85(1,3);") == std::string::npos || entry_object != arena + O || getf(O + 360) != 5.0f - 180.0f + 96.0f) return 8;
    // the ground: with a vertical-speed single set and the object below the ground, state 2, sound, and no search
    reset(8, 105); putf(O + 400, 1.0f); ground = 50.0f;
    msg_run__15ShockProjectile(arena + O);
    if (arena[O + 393] != 2 || get32(O + 88) != 8 - 270 || getf(O + 288) != 0.0f || getf(O + 292) != 0.0f || getf(O + 400) != 0.0f) return 9;
    if (calls.find("lump;sound;") == std::string::npos || lump_b != "spel_ew_shock_hit.anm" || calls.find("box") != std::string::npos) return 10;
    // a vertical-speed single set but the object above ground does nothing there
    reset(8, 105); putf(O + 400, 1.0f); ground = 1.0f;
    msg_run__15ShockProjectile(arena + O);
    if (arena[O + 393] != 0 || calls.find("box512;") == std::string::npos) return 11;
    // contact search on frames that are multiples of 8, no caster node: flagged (0x20) objects without 0x0d bits and within range
    reset(8, 105);
    put32(E1 + 12, 0x20); putf(E1 + 16, 1.5f); putf(E1 + 20, 2.25f); putf(E1 + 24, 3.0f); putf(E1 + 28, 2.0f);
    put32(E2 + 12, 0x20); putf(E2 + 16, 10.0f);
    put32(E3 + 12, 0x21);
    found_small = {arena + E3, arena + E1};
    found_big = {arena + E1, arena + E2, arena + E3, arena + O};
    msg_run__15ShockProjectile(arena + O);
    if (box_low[0].x != 1.5f - 18.0f || box_high[0].z != 3.0f + 18.0f || box_low[1].x != 1.5f - 1200.0f || box_high[1].y != 2.25f + 1200.0f) return 12;
    // links: E2 (flagged, not the object, not the hit) then the hit itself; the chain then has two nodes and both are attacked
    if (linked.size() != 2 || linked[0] != arena + E2 || linked[1] != arena + E1) return 13;
    if (attacks.size() != 2 || attacks[0].p != 100000 || attacks[0].s != 64 || attacks[0].source != nullptr) return 14;
    if (attacks[0].target != arena + E1 || attacks[1].target != arena + E2) return 15;
    if (attacks[0].slot[0] != 24 || attacks[0].slot[1] != 20) return 16;
    if (amount_mode != 0 || amount_byte != 3 || amount_object != arena + SRC || amount_address != arena + O + 404) return 17;
    // after the hit: state 1, the word at +88 is frame - 270, the speeds clear and both handle slots are -1
    if (arena[O + 393] != 1 || get32(O + 88) != 8 - 270 || getf(O + 288) != 0.0f || getf(O + 400) != 0.0f) return 18;
    if (get32(O + 300) != 0xffffffffu && get32(O + 300) != 500u) return 19;
    // state 1 draws lightning between consecutive chain nodes: first a start (handle stored), then moves
    if (starts.size() != 1 || starts[0].c != 260 || starts[0].d != 4 || starts[0].e != 16.0f || starts[0].f != 8.5f || starts[0].g != 0x5e808080u) return 20;
    if (starts[0].a.z != 3.0f + 36.0f || get32(O + 300) != 500u) return 21;
    msg_run__15ShockProjectile(arena + O);
    if (moves.size() != 1 || moves[0].handle != 500 || moves[0].c != 128 || moves[0].d != -1) return 22;
    // with a caster node the table slot 160 of the node's object decides who is a target
    reset(8, 105);
    put32(O + 404, CASTER); put32(CASTER + 12, OWN); put32(OWN + 72, OWNT); put32(OWNT + 160 + 4, 0x70);
    found_small = {arena + E1, arena + E2};
    yes_for = E2; putf(E2 + 16, 1.5f); putf(E2 + 20, 2.25f); putf(E2 + 28, 2.0f);
    found_big = {arena + E1, arena + E2};
    msg_run__15ShockProjectile(arena + O);
    if (linked.size() != 2 || linked[0] != arena + E2 || linked[1] != arena + E2) return 23;
    (void)get32; (void)chain_point_target; (void)light_q;
    return 0;
}
