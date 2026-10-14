// Host contract for msg_run__16beetleProjectile; not an EE input. 32-bit words are offsets from a host base and the callees log.
// The authored function was also compared with the original in a private R5900 run (random rolls with rejected draws, move
// results, the delay word against the frame number, the fire owner word and allocation results; every original word covered).
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
static bool alloc_ok;
struct Added { Point3 velocity; Point3 where; void* def; float age; };
static std::vector<Added> added;
struct Fire { Point3 where; void* owner; };
static std::vector<Fire> fires;
struct Mark { Point3 where; short a; float b; int c, d; };
static std::vector<Mark> marks;
static Point3 move_delta, height_probe;
static float height_radius, ground, sound_volume;
static void *sound_header, *sound_object;
static unsigned char block[100] __attribute__((aligned(16)));
extern "C" {
int frameNm;
void* world;
unsigned char norrath_w13_spell_msg_a_view_7d72b0[80];
unsigned char norrath_w13_spell_msg_a_view_7d7300[80];
int eRand__10GameObject(void*) { return rolls[rolls_used++ % rolls.size()]; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float f) { added.push_back({*v, *p, d, f}); }
int move__10ProjectileG6Point3(void*, Point3 delta) { calls += "move;"; move_delta = delta; return move_result; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) { calls += "sound;"; sound_header = h; sound_object = o; sound_volume = v; }
float worldFindHeight__FP12_worldHeaderG6Point3f(void*, Point3 p, float r) { calls += "height;"; height_probe = p; height_radius = r; return ground; }
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; return alloc_ok ? block : nullptr; }
void* __9LooseFireG6Point3P10GameObject(void* self, Point3 p, void* owner) { calls += "fire;"; fires.push_back({p, owner}); return self; }
void floorAdd__FG6Point3sfii(Point3 p, short a, float b, int c, int d) { calls += "mark;"; marks.push_back({p, a, b, c, d}); }
}
static unsigned char arena[0x2000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static float getf(unsigned off) { float v; std::memcpy(&v, arena + off, 4); return v; }
static unsigned get32(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
enum { O = 0x100, X = 0x700, OWN = 0x780, VT = 0x800 };
static void reset(int result, int frame, int delay) {
    w12_spell_c_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    calls.clear(); added.clear(); fires.clear(); marks.clear(); rolls = {49152}; rolls_used = 0; move_result = result; ground = 7.0f; alloc_ok = true;
    entry_object = nullptr; frameNm = frame;
    put32(O + 236, static_cast<unsigned>(delay)); put32(O + 72, VT); put32(VT + 8 + 4, 0x66); put32(O + 456, 0x900); put32(O + 452, 0);
    putf(O + 16, 1); putf(O + 20, 2); putf(O + 24, 3); putf(O + 428, 0.5f); putf(O + 432, 0.25f); putf(O + 436, 20.0f);
    put32(X + 12, OWN);
}
int main() {
    // before the delay: two fuse particles and one smoke particle, the countdown falls, no landing
    reset(0, 5, 100);
    msg_run__16beetleProjectile(arena + O);
    if (calls != "move;" || added.size() != 3 || rolls_used != 9) return 1;
    for (int i = 0; i < 2; ++i)
        if (added[i].def != norrath_w13_spell_msg_a_view_7d72b0 || added[i].velocity.x != 2.5f || added[i].velocity.y != 2.5f || added[i].velocity.z != 12.5f || added[i].where.x != 1.0f) return 2;
    if (added[2].def != norrath_w13_spell_msg_a_view_7d7300 || std::fabs(added[2].velocity.x - 0.64f) > 1e-3 || std::fabs(added[2].velocity.z - 0.64f * 24.72f) > 1e-2) return 3;
    if (std::fabs(getf(O + 436) - 11.0f) > 1e-4 || move_delta.x != 0.5f || move_delta.y != 0.25f || get32(O + 236) != 100) return 4;
    // a negative z draw is mirrored
    reset(0, 5, 100); rolls = {49152, 49152, 16384};
    msg_run__16beetleProjectile(arena + O);
    if (added[0].velocity.z != 12.5f) return 5;
    // move non-zero clears the delay word; with the frame past it the landing runs: sound, five passes, entry
    reset(1, 5, 100);
    msg_run__16beetleProjectile(arena + O);
    if (get32(O + 236) != 0) return 6;
    if (calls.substr(0, 11) != "move;sound;" || calls.find("entry102(1,3);") == std::string::npos || sound_header != arena + 0x900 || sound_object != arena + O || sound_volume != 1.0f) return 7;
    if (marks.size() != 5 || height_radius != 6.0f || fires.size() != 0) return 8;
    for (auto& m : marks) if (m.b != 24.0f || m.c != 9 || m.d != 0 || m.where.z != 9.0f || m.a != static_cast<short>(49152)) return 9;
    // each pass puts the point 20 * unit away from the object (0.5 -> 10) and lifts to height + 1
    if (marks[0].where.x != 11.0f || marks[0].where.y != 12.0f) return 10;
    // with an owner word, a fire block is allocated and built each pass
    reset(0, 200, 100); put32(O + 452, X);
    msg_run__16beetleProjectile(arena + O);
    if (fires.size() != 5 || fires[0].owner != arena + OWN || fires[0].where.z != 8.0f || calls.find("alloc84;") == std::string::npos) return 11;
    // allocation failure skips the fire but still adds the mark
    reset(0, 200, 100); put32(O + 452, X); alloc_ok = false;
    msg_run__16beetleProjectile(arena + O);
    if (fires.size() != 0 || marks.size() != 5) return 12;
    // frame equal to the delay still lands, one below does not
    reset(0, 99, 100);
    msg_run__16beetleProjectile(arena + O);
    if (!marks.empty()) return 13;
    reset(0, 100, 100);
    msg_run__16beetleProjectile(arena + O);
    if (marks.size() != 5) return 14;
    return 0;
}
