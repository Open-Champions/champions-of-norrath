// Host contract for msg_run__11DiseaseBolt; not an EE input. 32-bit words are offsets from a host base and the callees log. The
// authored function was also compared with the original in a private R5900 run (see below); the table call goes through the
// shared helper, which a host contract routes to a logging hook. The comparison covered random rolls with rejected draws, the
// target presence and turn limits, the move result bits, and the frame limit; every original word was covered.
#define W12_SPELL_C_HOST 1
#include "msg_run.cpp"
#include <cmath>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w12_spell_c_host_base;
static std::string calls;
static void* entry_object;
unsigned w12_spell_c_host_call(unsigned fn, void* object, int count, unsigned long a1, unsigned long) {
    calls += "entry" + std::to_string(fn) + "(" + std::to_string(count) + "," + std::to_string(a1) + ");";
    entry_object = object;
    return 0;
}
static int limit_result, limit_mode, limit_halfword;
static void *limit_object, *limit_address;
int w14b_host_call3(unsigned, void* adjusted, int first, int second, void* third) {
    calls += "limit;"; limit_object = adjusted; limit_mode = first; limit_halfword = second; limit_address = third; return limit_result;
}
static std::vector<int> rolls;
static size_t rolls_used;
struct Added { Point3 where, velocity; void* def; float age; };
static std::vector<Added> added;
static int atan_result, move_result;
static float atan_y, atan_x;
static Point3 move_delta, effect_at;
static int effect_kind;
static std::string lump_a, lump_b;
static float sound_volume;
static void *sound_object;
extern "C" {
int frameNm;
unsigned char norrath_w13_spell_msg_a_view_8d67d0[80];
unsigned char norrath_w13_spell_msg_a_view_8d6820[80];
unsigned char norrath_w13_spell_msg_a_view_8d6870[80];
int eRand__10GameObject(void*) { return rolls[rolls_used++ % rolls.size()]; }
float icos__Fi(int a) { return static_cast<float>(a); }
float isin__Fi(int a) { return static_cast<float>(a) * 2.0f; }
int iatan2__Fff(float y, float x) { calls += "atan;"; atan_y = y; atan_x = x; return atan_result; }
int move__10ProjectileG6Point3(void*, Point3 delta) { calls += "move;"; move_delta = delta; return move_result; }
void* lumpFindResource__FPcT0(char* a, char* b) { calls += "lump;"; lump_a = a; lump_b = b; return nullptr; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void* o, float v) { calls += "sound;"; sound_object = o; sound_volume = v; }
void addEffect__FiG6Point3(int kind, Point3 p) { calls += "effect;"; effect_kind = kind; effect_at = p; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float f) { added.push_back({*p, *v, d, f}); }
}
static unsigned char arena[0x2000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static void put16(unsigned off, short v) { std::memcpy(arena + off, &v, 2); }
static float getf(unsigned off) { float v; std::memcpy(&v, arena + off, 4); return v; }
static short get16(unsigned off) { short v; std::memcpy(&v, arena + off, 2); return v; }
static unsigned get32(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
enum { O = 0x100, TN = 0x700, TO = 0x780, CASTER = 0x800, VT = 0x880 };
static void reset(int frame, int end) {
    w12_spell_c_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    calls.clear(); added.clear(); rolls = {49152}; rolls_used = 0; entry_object = nullptr;
    limit_result = 3; atan_result = 0; move_result = 0; frameNm = frame;
    putf(O + 16, 1); putf(O + 20, 2); putf(O + 24, 3);
    put32(O + 112, static_cast<unsigned>(end)); put32(O + 116, 0); put32(O + 120, 0);
    put32(O + 72, VT); put32(VT + 8 + 4, 0x55);
    put32(O + 88, 0); put32(O + 92, CASTER); put16(O + 96, 9);
}
int main() {
    // flying, no target: 16 particles in the order 5 x def1, 5 x def2, 6 x def3 with the stated scales
    reset(10, 100);
    msg_run__11DiseaseBolt(arena + O);
    if (added.size() != 16 || calls != "move;" || rolls_used != 96) return 1;
    for (int i = 0; i < 16; ++i) {
        void* want = i < 5 ? static_cast<void*>(norrath_w13_spell_msg_a_view_8d67d0) : i < 10 ? static_cast<void*>(norrath_w13_spell_msg_a_view_8d6820) : static_cast<void*>(norrath_w13_spell_msg_a_view_8d6870);
        const float ps = i < 5 ? 10.0f : 15.0f, vs = i < 5 ? 16.0f : 5.0f;
        if (added[i].def != want || added[i].where.x != 1.0f + 0.5f * ps || added[i].velocity.y != 0.5f * vs || added[i].age != 0.0f) return 2;
    }
    // target present and old enough: turn limit from the caster slot (3 * 182), the heading turns by at most the limit
    reset(10, 100); put32(O + 88, TN); put32(TN + 12, TO); putf(TO + 16, 21.0f); putf(TO + 20, 32.0f); atan_result = 10000;
    msg_run__11DiseaseBolt(arena + O);
    if (limit_mode != 1 || limit_halfword != 9 || limit_object != arena + CASTER || limit_address != arena + O + 76) return 3;
    if (atan_x != 20.0f || atan_y != 30.0f || get16(O + 56) != 546) return 4;
    if (getf(O + 100) != 546.0f * 350.0f || getf(O + 104) != 546.0f * 2.0f * 350.0f || get32(O + 108) != 0) return 5;
    reset(10, 100); put32(O + 88, TN); put32(TN + 12, TO); atan_result = -10000; put16(O + 56, 100);
    msg_run__11DiseaseBolt(arena + O);
    if (get16(O + 56) != 100 - 546) return 6;
    reset(10, 100); put32(O + 88, TN); put32(TN + 12, TO); atan_result = 300; put16(O + 56, 100);
    msg_run__11DiseaseBolt(arena + O);
    if (get16(O + 56) != 300) return 7;
    // a target whose node age is not behind the frame is not chased
    reset(10, 100); put32(O + 88, TN); put32(TN + 12, TO); put32(O + 116, 10);
    msg_run__11DiseaseBolt(arena + O);
    if (calls.find("atan") != std::string::npos) return 8;
    // collision bits: bit 1 always ends it; bit 2 only with the word at +120
    reset(10, 100); move_result = 1;
    msg_run__11DiseaseBolt(arena + O);
    if (get32(O + 112) != 0 || calls.find("lump;") == std::string::npos) return 9;
    reset(10, 100); move_result = 2;
    msg_run__11DiseaseBolt(arena + O);
    if (get32(O + 112) != 100 || calls.find("lump;") != std::string::npos) return 10;
    reset(10, 100); move_result = 2; put32(O + 120, 1);
    msg_run__11DiseaseBolt(arena + O);
    if (get32(O + 112) != 0 || calls.find("lump;") == std::string::npos) return 11;
    // burst: hit sound, effect 12 at the object, 30 more particles (10 x three definitions), then table entry 8 with 3
    if (lump_a != "fx.lmp" || lump_b != "spel_de_disbolt_hit.anm" || sound_volume != 1.0f || sound_object != arena + O || effect_kind != 12 || effect_at.x != 1.0f) return 12;
    if (added.size() != 16 + 30 || calls.substr(calls.size() - 13) != "entry85(1,3);") return 13;
    if (added[16].where.x != 1.0f + 0.5f * 10.0f || added[16].velocity.x != 0.5f * 96.0f || added[17].velocity.x != 0.5f * 30.0f || added[17].def != norrath_w13_spell_msg_a_view_8d6820) return 14;
    return 0;
}
