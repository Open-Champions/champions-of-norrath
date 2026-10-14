// Host contract for msg_run__16PoisonProjectile; not an EE input. 32-bit words are offsets from a host base and the callees log.
// The authored function was also compared with the original in a private R5900 run (random
// rolls with rejected draws, move results 0/1/2/7/-1, owner presence, heights and difficulty scales; every original word covered).
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
static std::vector<int> rolls;
static size_t rolls_used;
static int move_result;
static float ground, difficulty;
struct Added { Point3 velocity; float age; void* def; };
static std::vector<Added> added;
static Point3 move_delta, floor_at, radial_at, effect_at, height_probe;
static short floor_a; static float floor_b; static int floor_c, floor_d, radial_amount, effect_kind;
static float radial_radius, radial_f, height_radius;
static void *radial_owner, *sound_header, *sound_object;
extern "C" {
int frameNm;
void* world;
unsigned char norrath_w13_spell_msg_a_view_7d7260[80];
int eRand__10GameObject(void*) { return rolls[rolls_used++ % rolls.size()]; }
int animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(void*, void*, void*, short*) { calls += "anim;"; return 0; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3*, Point3* v, float f) { added.push_back({*v, f, d}); }
int move__10ProjectileG6Point3(void*, Point3 delta) { calls += "move;"; move_delta = delta; return move_result; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) { calls += v == 1.0f ? "sound;" : "badvolume;"; sound_header = h; sound_object = o; }
float worldFindHeight__FP12_worldHeaderG6Point3f(void*, Point3 p, float r) { calls += "height;"; height_probe = p; height_radius = r; return ground; }
void floorAdd__FG6Point3sfii(Point3 p, short a, float b, int c, int d) { calls += "floor;"; floor_at = p; floor_a = a; floor_b = b; floor_c = c; floor_d = d; }
float gameDifficultyScale__Fv() { calls += "scale;"; return difficulty; }
void objectRadialDamage__FP10GameObjectG6Point3iffiii(void* owner, Point3 p, int amount, float radius, float f, int, int, int) {
    calls += "radial;"; radial_owner = owner; radial_at = p; radial_amount = amount; radial_radius = radius; radial_f = f;
}
void addEffect__FiG6Point3(int kind, Point3 p) { calls += "effect;"; effect_kind = kind; effect_at = p; }
}
static unsigned char arena[0x2000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static float getf(unsigned off) { float v; std::memcpy(&v, arena + off, 4); return v; }
static unsigned get32(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
enum { O = 0x100, TB = 0x500, Q = 0x700, OWN = 0x780, VT = 0x800 };
static void reset(int result) {
    w12_spell_c_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    calls.clear(); added.clear(); rolls = {49152}; rolls_used = 0; move_result = result; ground = 0.0f; difficulty = 2.0f; entry_object = nullptr;
    frameNm = 5;
    put32(O + 36, TB); put32(O + 72, VT); put32(O + 96, 1); put32(O + 76, 0); put32(O + 152, 0); put32(O + 156, 100);
    put32(TB + 8 + 4, 0x3000);
    put32(VT + 8 + 4, 0x55);
    putf(O + 16, 1); putf(O + 20, 2); putf(O + 24, 3); putf(O + 140, 0.5f); putf(O + 144, 0.25f); putf(O + 148, 10.0f); putf(O + 164, 100.0f);
    put32(Q + 12, OWN);
}
int main() {
    // no collision: four particles with velocity unit*8, the single at +148 falls, no entry call yet
    reset(0);
    msg_run__16PoisonProjectile(arena + O);
    if (calls != "anim;move;" || added.size() != 4 || rolls_used != 12) return 1;
    for (auto& a : added) if (a.velocity.x != 4.0f || a.velocity.y != 4.0f || a.velocity.z != 4.0f || a.age != 0.0f || a.def != norrath_w13_spell_msg_a_view_7d7260) return 2;
    if (std::fabs(getf(O + 148) - (10.0f - 4.95f)) > 1e-4 || move_delta.x != 0.5f || move_delta.y != 0.25f) return 3;
    // a draw outside the ball is repeated
    reset(0); rolls = {65535, 65535, 65535, 49152, 49152, 49152};
    msg_run__16PoisonProjectile(arena + O);
    if (added.size() != 4 || rolls_used != 24 || added[0].velocity.x != 4.0f) return 4;
    // frame reaches the stored limit: entry at offset 8 is called with 3
    reset(0); frameNm = 100;
    msg_run__16PoisonProjectile(arena + O);
    if (calls != "anim;move;entry85(1,3);" || entry_object != arena + O) return 5;
    // a hit that is not exactly 1 plays the sound and clears the limit word; no owner, no damage
    reset(7);
    msg_run__16PoisonProjectile(arena + O);
    if (calls != "anim;move;sound;entry85(1,3);" || get32(O + 156) != 0 || get32(O + 152) != 0 || sound_header != arena + 0x3000 || sound_object != arena + O) return 6;
    // result 1 lifts the point to the ground, sets the flag, adds a floor mark when below the limit - 24, and damages the owner
    reset(1); put32(O + 76, Q); ground = 9.0f; rolls = {49152, 49152, 49152, 49152, 49152, 49152, 49152, 49152, 49152, 49152, 49152, 49152, 49152, 7, 4};
    msg_run__16PoisonProjectile(arena + O);
    if (calls != "anim;move;sound;height;floor;scale;radial;effect;entry85(1,3);") return 7;
    if (getf(O + 24) != 9.0f || get32(O + 152) != 1 || height_radius != 5.0f) return 8;
    if (floor_a != static_cast<short>(49152)) return 9;
    if (floor_b != 12.0f + 7 % 6 || floor_c != 4 || floor_d != 0) return 10;
    if (radial_owner != arena + OWN || radial_amount != 4 || radial_radius != 12.0f || radial_f != 40.0f || effect_kind != 6 || radial_at.x != 1.0f) return 11;
    // ground above the limit - 24 adds no floor mark
    reset(1); put32(O + 76, Q); ground = 80.0f; putf(O + 164, 100.0f);
    msg_run__16PoisonProjectile(arena + O);
    if (calls != "anim;move;sound;height;scale;radial;effect;entry85(1,3);" || getf(O + 24) != 80.0f) return 12;
    // a result of 2 keeps an earlier flag word and still damages the owner
    reset(2); put32(O + 76, Q); put32(O + 152, 1);
    msg_run__16PoisonProjectile(arena + O);
    if (calls != "anim;move;sound;scale;radial;effect;entry85(1,3);") return 13;
    return 0;
}
