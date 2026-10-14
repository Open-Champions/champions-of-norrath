// Host contract for msg_run__14poisonGasSpell; not an EE input. 32-bit words are offsets from a host base and the callees log.
// The authored function was also compared with the original in a private R5900 run (random rolls with rejected draws, end
// frames, particle counts, frame multiples of 30, list contents and slot results; every original word covered).
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
static std::vector<int> rolls;
static size_t rolls_used;
struct Added { Point3 velocity, where; void* def; float age; };
static std::vector<Added> added;
static Point3 box_low, box_high;
static int box_capacity;
static std::vector<void*> found;
struct Attack { void* target; void* source; int p, s; short slot[4]; };
static std::vector<Attack> attacks;
extern "C" {
int frameNm;
unsigned char norrath_w13_spell_msg_a_view_7d6970[80];
int eRand__10GameObject(void*) { return rolls[rolls_used++ % rolls.size()]; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float f) { added.push_back({*v, *p, d, f}); }
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3* low, Point3* high, void** list, int capacity) {
    calls += "box;"; box_low = *low; box_high = *high; box_capacity = capacity;
    for (size_t i = 0; i < found.size(); ++i) list[i] = found[i];
    list[found.size()] = nullptr;
}
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void* b, float, float, int p, int, int, int s, void* d, bool) {
    calls += "attack;";
    Attack at{a, b, p, s, {}};
    std::memcpy(at.slot, static_cast<unsigned char*>(d) + 36, 8);
    attacks.push_back(at);
}
}
static unsigned char arena[0x2000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static float getf(unsigned off) { float v; std::memcpy(&v, arena + off, 4); return v; }
enum { O = 0x100, Q = 0x700, OWN = 0x780, OT = 0x800, VT = 0xa00, T1 = 0xc00, T2 = 0xc40 };
static void reset(int frame, int end, int n, float reach) {
    w12_spell_c_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    calls.clear(); added.clear(); attacks.clear(); found.clear(); rolls = {49152}; rolls_used = 0; entry_object = nullptr;
    yes_for = 0xffffffffu;
    frameNm = frame;
    put32(O + 92, static_cast<unsigned>(end)); put32(O + 96, static_cast<unsigned>(n)); putf(O + 100, reach); put32(O + 76, 0);
    put32(O + 72, VT); put32(VT + 8 + 4, 0x55);
    put32(O + 108, 0x12340007);
    putf(O + 16, 1); putf(O + 20, 2); putf(O + 24, 3);
    put32(Q + 12, OWN); put32(OWN + 72, OT); put32(OT + 160 + 4, 0x70);
}
int main() {
    // growing cloud: reach rises by 0.5 up to the count, n/10 particles at the reach from the object, velocity unit*5
    reset(7, 50, 25, 0.0f);
    msg_run__14poisonGasSpell(arena + O);
    if (getf(O + 100) != 0.5f || added.size() != 2 || rolls_used != 12 || !calls.empty()) return 1;
    for (auto& a : added) if (a.where.x != 1.25f || a.where.y != 2.25f || a.where.z != 3.25f || a.velocity.x != 2.5f || a.age != 0.0f ||
                              a.def != norrath_w13_spell_msg_a_view_7d6970) return 2;
    // at the full reach it stops growing; fewer than ten means no particles
    reset(7, 50, 25, 25.0f);
    msg_run__14poisonGasSpell(arena + O);
    if (getf(O + 100) != 25.0f) return 3;
    reset(7, 50, 9, 0.0f);
    msg_run__14poisonGasSpell(arena + O);
    if (!added.empty() || getf(O + 100) != 0.5f) return 4;
    // frame multiple of 30 with an owner: box search of half-size (int)reach, entry 160 per object, attack when it returns non-zero
    reset(30, 50, 20, 5.0f); put32(O + 76, Q);
    found = {arena + T1, arena + T2};
    yes_for = T1;
    msg_run__14poisonGasSpell(arena + O);
    if (getf(O + 100) != 5.5f || box_capacity != 64 || box_low.x != -4.0f || box_high.z != 8.0f) return 5;
    if (attacks.size() != 1 || attacks[0].target != arena + T1 || attacks[0].source != nullptr || attacks[0].p != 100000 || attacks[0].s != 0x01000040) return 6;
    if (attacks[0].slot[0] != 23 || attacks[0].slot[1] != 7 || attacks[0].slot[2] != 7 || attacks[0].slot[3] != 0) return 7;
    // other frames, or no owner, skip the search
    reset(31, 50, 20, 5.0f); put32(O + 76, Q);
    msg_run__14poisonGasSpell(arena + O);
    if (calls.find("box") != std::string::npos) return 8;
    reset(30, 50, 20, 5.0f);
    msg_run__14poisonGasSpell(arena + O);
    if (calls.find("box") != std::string::npos) return 9;
    // end of life: only the entry at offset 8 runs, with 3
    reset(50, 50, 20, 5.0f); put32(O + 76, Q);
    msg_run__14poisonGasSpell(arena + O);
    if (calls != "entry85(1,3);" || entry_object != arena + O || !added.empty() || getf(O + 100) != 5.0f) return 10;
    return 0;
}
