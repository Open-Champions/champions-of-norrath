// Host contract for msg_run__10PoisonRain; not an EE input. 32-bit words are offsets from a host base and the callees log. The
// authored function was also compared with the original in a private R5900 run (random rolls, reaches around the particle-count
// boundaries, frame multiples of 30, list contents, slot results and allocation results; every original word covered).
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
static int amount_result, amount_mode, amount_halfword;
static void* amount_object;
static void* amount_address;
int w14b_host_call3(unsigned fn, void* adjusted, int first, int second, void* third) {
    calls += "amount;"; (void)fn; amount_object = adjusted; amount_mode = first; amount_halfword = second; amount_address = third;
    return amount_result;
}
static std::vector<int> rolls;
static size_t rolls_used;
struct Added { Point3 where, velocity; void* def; float age; };
static std::vector<Added> added;
static Point3 box_low, box_high;
static std::vector<void*> found;
struct Attack { void* target; void* source; int p, s; short slot[5]; };
static std::vector<Attack> attacks;
struct Light { Point3 where; float f[12]; void* object; int flags; };
static std::vector<Light> lights;
static bool alloc_ok;
static int stop_channel;
static float ground;
static unsigned char block[200] __attribute__((aligned(16)));
extern "C" {
int frameNm;
void* world;
unsigned char norrath_w13_spell_msg_a_view_8d8190[80];
unsigned char norrath_w13_spell_msg_a_view_8d81e0[80];
int eRand__10GameObject(void*) { return rolls[rolls_used++ % rolls.size()]; }
float icos__Fi(int) { return 1.0f; }
float isin__Fi(int) { return 0.5f; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float f) { added.push_back({*p, *v, d, f}); }
float worldFindPointHeight__FP12_worldHeaderG6Point3(void*, Point3*) { calls += "ground;"; return ground; }
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3* low, Point3* high, void** list, int capacity) {
    calls += "box" + std::to_string(capacity) + ";"; box_low = *low; box_high = *high;
    for (size_t i = 0; i < found.size(); ++i) list[i] = found[i];
    list[found.size()] = nullptr;
}
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void* b, float, float, int p, int, int, int s, void* d, bool) {
    calls += "attack;";
    Attack at{a, b, p, s, {}};
    std::memcpy(at.slot, static_cast<unsigned char*>(d) + 36, 8);
    attacks.push_back(at);
}
void soundStopSound__Fi(int c) { calls += "stop;"; stop_channel = c; }
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; return alloc_ok ? block : nullptr; }
void lightEffect_construct(void*, const Point3* p, float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, void* object, int flags)
    __asm__("__11LightEffectG6Point3ffffffffffffP10GameObjecti");
void lightEffect_construct(void*, const Point3* p, float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, void* object, int flags) {
    calls += "light;";
    lights.push_back({*p, {a, b, c, d, e, f, g, h, i, j, k, l}, object, flags});
}
}
static unsigned char arena[0x3000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
enum { O = 0x100, Z = 0x700, ZT = 0x780, VT = 0x800, A1 = 0x900, A2 = 0x940, SRC = 0xa00, SRCT = 0xa80 };
static void reset(int frame, int start, int end, float reach) {
    w12_spell_c_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    calls.clear(); added.clear(); attacks.clear(); lights.clear(); found.clear(); rolls = {49152}; rolls_used = 0; entry_object = nullptr;
    yes_for = 0xffffffffu; alloc_ok = true; ground = 4.0f; amount_result = 20;
    frameNm = frame;
    put32(O + 80, static_cast<unsigned>(start)); put32(O + 76, static_cast<unsigned>(end)); putf(O + 84, reach); put32(O + 100, Z - 12 + 0);
    put32(O + 72, VT); put32(VT + 8 + 4, 0x55);
    putf(O + 16, 1); putf(O + 20, 2); putf(O + 24, 3);
    put32(Z + 0, 0); put32(O + 100, Z - 12);
    put32(Z - 12 + 12, Z); put32(Z + 72, ZT); put32(ZT + 160 + 4, 0x70);
    put32(O + 96, SRC); put32(SRC + 96, SRCT); put32(O + 92, 5); put32(O + 104, 77);
}
int main() {
    // not yet started: nothing happens at all
    reset(10, 10, 100, 10.0f);
    msg_run__10PoisonRain(arena + O);
    if (!calls.empty() || !added.empty()) return 1;
    // running: 9 cloud drops and 5 splashes at reach 10
    reset(10, 5, 100, 10.0f);
    msg_run__10PoisonRain(arena + O);
    int drops = 0, splashes = 0;
    for (auto& a : added) {
        if (a.def == norrath_w13_spell_msg_a_view_8d8190) ++drops;
        if (a.def == norrath_w13_spell_msg_a_view_8d81e0) ++splashes;
    }
    if (drops != 9 || splashes != 5) return 2;
    // drop: x = icos * (t * R) + P.x with t = 0.75; z = ((200 + r3 % 200) - 100) + P.z; velocity (0, 0, -80)
    if (added[0].where.x != 1.0f + 7.5f || added[0].where.y != 2.0f + 0.5f * 7.5f || added[0].where.z != ((200.0f + (49152 % 200)) - 100.0f) + 3.0f) return 3;
    if (added[0].velocity.x != 0.0f || added[0].velocity.z != -80.0f || added[0].age != 0.0f) return 4;
    // splash: velocity (rolls & 1) - 1 = -1, z = ground + 5
    if (added[9].velocity.x != -1.0f || added[9].velocity.y != -1.0f || added[9].velocity.z != 0.0f || added[9].where.z != 9.0f) return 5;
    // small reaches still make one drop and one splash; zero reach makes none
    reset(10, 5, 100, 1.0f);
    msg_run__10PoisonRain(arena + O);
    if (added.size() != 2) return 6;
    reset(10, 5, 100, 0.0f);
    msg_run__10PoisonRain(arena + O);
    if (!added.empty()) return 7;
    // every 30th frame: box search of (int)reach, slot 160 on the owner object per entry, an attack where it says yes
    reset(30, 5, 100, 6.5f);
    found = {arena + 0xb00, arena + 0xb40};
    yes_for = 0xb40;
    msg_run__10PoisonRain(arena + O);
    if (box_low.x != -5.0f || box_high.y != 8.0f) return 8;
    if (attacks.size() != 1 || attacks[0].target != arena + 0xb40 || attacks[0].source != arena + Z || attacks[0].p != 100000 || attacks[0].s != 64) return 9;
    if (amount_object != arena + SRC || amount_mode != 0 || amount_halfword != 5 || amount_address != arena + O + 100) return 10;
    if (attacks[0].slot[0] != 23 || attacks[0].slot[1] != 20) return 11;
    // end of life: sound stop, light flash at (x, y, z + 12), entry 8 with 3
    reset(10, 5, 10, 10.0f);
    msg_run__10PoisonRain(arena + O);
    if (calls != "stop;alloc188;light;entry85(1,3);" || stop_channel != 77 || lights.size() != 1) return 12;
    if (lights[0].where.x != 1.0f || lights[0].where.z != 15.0f || lights[0].f[1] != 255.0f || lights[0].f[8] != 20.0f || lights[0].flags != 1 || lights[0].object != nullptr) return 13;
    reset(10, 5, 10, 10.0f); alloc_ok = false;
    msg_run__10PoisonRain(arena + O);
    if (calls != "stop;alloc188;entry85(1,3);") return 14;
    // an owner word of zero also ends it
    reset(10, 5, 100, 10.0f); put32(O + 100, 0);
    msg_run__10PoisonRain(arena + O);
    if (calls.find("stop;") == std::string::npos) return 15;
    return 0;
}
