// Host contract for msg_run__9dustCloud; not an EE input. 32-bit words are offsets from a host base and the callees log.
// The authored function was also compared with the original in a private R5900 run (116 scenarios over frameNm, the limit,
// N and G, the word at +100, list contents and Cyclops-typed entries; every original word covered except the three of the
// null-pointer type test that cannot be reached).
#define W13_SPELL_MSG_B_HOST 1
#include "../w13-spell-msg-b-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w13_spell_msg_b_host_base;
static std::string calls;
static std::vector<int> rolls;
static unsigned roll_index;
static unsigned type_of_object[8];
unsigned w13_spell_msg_b_host_call(unsigned fn, void*, int, unsigned long a1, unsigned long) {
    if (fn >= 0x7000) return type_of_object[fn - 0x7000];
    calls += "entry" + std::to_string(fn) + "(" + std::to_string(a1) + ");";
    return 0;
}
struct Added { void* def; Point3 position, velocity; float age; };
static std::vector<Added> added;
static std::vector<unsigned> listed, attacked;
static Point3 box_low, box_high;
static int box_capacity, attack_flags;
static float attack_amount;
extern "C" {
int frameNm;
unsigned char dustcloud_msg_run_particle_7d69c0[8];
int eRand__10GameObject(void*) { return rolls[roll_index++ % rolls.size()]; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float a) { added.push_back({d, *p, *v, a}); }
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** found, int capacity) {
    calls += "box;"; box_low = low; box_high = high; box_capacity = capacity;
    for (size_t i = 0; i < listed.size(); ++i) found[i] = w13_spell_msg_b_host_base + listed[i];
    found[listed.size()] = nullptr;
}
void* __tf7Cyclops() { return w13_spell_msg_b_host_base + 0x300; }
bool __eq__C9type_infoRC9type_info(const void* a, const void* b) { calls += "eq;"; return a == b; }
int gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void* b, float x, float y, int p, int, int, int s, void* description, bool) {
    calls += "attack;"; attack_flags = s; attack_amount = x;
    attacked.push_back(static_cast<unsigned>(static_cast<unsigned char*>(a) - w13_spell_msg_b_host_base));
    if (b != nullptr || y != x || p != 100000 || description != nullptr) calls += "BAD;";
    return 0;
}
}
#include "msg_run.cpp"

static unsigned char arena[0x4000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static float getf(unsigned off) { float v; std::memcpy(&v, arena + off, 4); return v; }
enum { O = 0x100, OT = 0x800, A = 0x1000, B = 0x1400, TA = 0x1800, TB = 0x1900 };
static void reset(int frame, int limit, int n, float g) {
    w13_spell_msg_b_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    calls.clear(); added.clear(); listed.clear(); attacked.clear(); roll_index = 0; rolls = {49152}; frameNm = frame;
    put32(O + 88, limit); put32(O + 92, n); putf(O + 96, g); put32(O + 104, 7);
    put32(O + 72, OT); put32(OT + 8 + 4, 0x900);
    putf(O + 16, 1); putf(O + 20, 2); putf(O + 24, 3);
    put32(A + 72, TA); put32(TA + 4, 0x7000); type_of_object[0] = 0x300;  // A is a Cyclops
    put32(B + 72, TB); put32(TB + 4, 0x7001); type_of_object[1] = 0x340;  // B is not
}
int main() {
    unsigned char* const o = arena + O;
    reset(5, 5, 100, 1.0f);  // frameNm is not below the limit: entry 8 with 3
    msg_run__9dustCloud(o);
    if (calls != "entry2304(3);" || !added.empty()) { std::puts(calls.c_str()); return 1; }
    // N = 25: c = 2 passes; G (24.0) < 25 grows by 0.5; D = (0.5,0.5,0.5), E * 5 = 2.5
    reset(7, 100, 25, 24.0f);
    msg_run__9dustCloud(o);
    if (getf(O + 96) != 24.5f || added.size() != 2 || !calls.empty()) return 2;
    for (const Added& a : added) {
        if (a.def != static_cast<void*>(dustcloud_msg_run_particle_7d69c0) || a.age != 0.0f) return 3;
        if (a.position.x != 0.5f * 24.5f + 1.0f || a.position.y != 0.5f * 24.5f + 2.0f || a.position.z != 0.5f * 24.5f + 3.0f) return 4;
        if (a.velocity.x != 2.5f || a.velocity.y != 2.5f || a.velocity.z != 2.5f) return 5;
    }
    // G at the limit does not grow; no passes for N below 10
    reset(7, 100, 9, 9.0f);
    msg_run__9dustCloud(o);
    if (getf(O + 96) != 9.0f || !added.empty()) return 6;
    // frameNm multiple of 60: the box and the attacks. G = 34.7 -> r = 34
    reset(60, 1000, 0, 34.7f); listed = {A, B};
    msg_run__9dustCloud(o);
    if (calls != "box;eq;eq;attack;") { std::puts(calls.c_str()); return 7; }
    if (box_low.x != 1.0f - 34.0f || box_high.z != 3.0f + 34.0f || box_capacity != 64) return 8;
    if (attacked.size() != 1 || attacked[0] != B || attack_flags != 0x5000040 || attack_amount != 7.0f) return 9;
    // the word at +100 set: nothing is skipped
    reset(60, 1000, 0, 34.7f); listed = {A, B}; put32(O + 100, 1);
    msg_run__9dustCloud(o);
    if (calls != "box;attack;attack;" || attacked.size() != 2) { std::puts(calls.c_str()); return 10; }
    // another frame: no box; an empty list ends quietly
    reset(61, 1000, 0, 1.0f); listed = {A};
    msg_run__9dustCloud(o);
    if (!calls.empty()) return 11;
    reset(-60, 1000, 0, 1.0f);
    msg_run__9dustCloud(o);
    if (calls != "box;") return 12;
    return 0;
}
