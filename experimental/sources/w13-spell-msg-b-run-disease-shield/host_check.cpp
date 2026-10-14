// Host contract for run__18SpellDiseaseShieldG6Point3s; not an EE input. 32-bit words are offsets from a host base and the
// callees log. The authored function was also compared with the original in a private R5900 run (261 scenarios over
// frameNm, the end word, the growth float and its limit, list contents, the 160 entry result, roll lists, an argument that
// aliases the object, and a sweep of radii around the comparison threshold of the sixteen points; every original word
// covered except nine unreachable ones: slots after the first of the description and delay-slot nops).
#define W13_SPELL_MSG_B_HOST 1
#include "../w13-spell-msg-b-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w13_spell_msg_b_host_base;
static std::string calls;
static int w160_result = 1, e104_result = 40;
static std::vector<int> rolls;
static unsigned roll_index;
unsigned w13_spell_msg_b_host_call(unsigned fn, void*, int, unsigned long a1, unsigned long a2) {
    if (fn == 0x700) { calls += "w160;"; return w160_result; }
    if (fn == 0x710) { calls += "e104(" + std::to_string(static_cast<int>(a1)) + "," + std::to_string(static_cast<int>(a2)) + ");"; return e104_result; }
    calls += "entry" + std::to_string(fn) + "(" + std::to_string(a1) + ");";
    return 0;
}
struct Added { void* def; Point3 position, velocity; };
static std::vector<Added> added;
static int faces, attacks, attack_flags;
static float attack_a, attack_b;
static short slot[4];
static std::vector<unsigned> listed;
static Point3 box_low, box_high;
static std::string lump_a, lump_b;
static Point3 sound_at;
static float sound_volume;
extern "C" {
int frameNm;
void* haloTexture5 = reinterpret_cast<void*>(0x77);
unsigned char diseaseshield_run_particle_8d69b0[8], diseaseshield_run_particle_8d6a00[8];
unsigned eRand__5Spell(void*) { return rolls[roll_index++ % rolls.size()]; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float) { added.push_back({d, *p, *v}); }
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3, float size, float size2, int a, int b, int c, int d, void* texture, short spin, int flag) {
    ++faces;
    if (size != 30.0f || size2 != 400.0f || a != 0 || b != 0 || c != 0 || d != 200 || texture != haloTexture5 || spin != 0 || flag != 0) calls += "BADFACE;";
}
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** found, int capacity) {
    calls += "box;"; box_low = low; box_high = high; if (capacity != 64) calls += "BADCAP;";
    for (size_t i = 0; i < listed.size(); ++i) found[i] = w13_spell_msg_b_host_base + listed[i];
    found[listed.size()] = nullptr;
}
int gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void*, void*, float x, float y, int, int, int s, int, void* description, bool) {
    (void)s; ++attacks; attack_a = x; attack_b = y;
    std::memcpy(slot, static_cast<unsigned char*>(description) + 36, 8);
    return 0;
}
void* lumpFindResource__FPcT0(char* a, char* b) { lump_a = a; lump_b = b; calls += "lump;"; return reinterpret_cast<void*>(0x55); }
void soundPlaySequence__FP15AnimationHeaderG6Point3f(void*, Point3 p, float v) { calls += "sound;"; sound_at = p; sound_volume = v; }
}
#include "run.cpp"

static unsigned char arena[0x4000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static float getf(unsigned off) { float v; std::memcpy(&v, arena + off, 4); return v; }
enum { O = 0x100, W = 0x900, X2 = 0xa00, TW = 0xb00, TX = 0xc00, OT = 0xd00, A = 0x1000, B = 0x1400 };
static void reset(int frame, int end, int limit, float growth) {
    w13_spell_msg_b_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    calls.clear(); added.clear(); listed.clear(); faces = attacks = 0; roll_index = 0; rolls = {49152}; frameNm = frame; w160_result = 1; e104_result = 40;
    put32(O + 420, end); put32(O + 424, limit); putf(O + 428, growth); put32(O + 12, W); put32(O + 8, X2); put32(O + 20, OT);
    put32(OT + 8 + 4, 0x900); put32(W + 72, TW); put32(TW + 160 + 4, 0x700); put32(X2 + 96, TX); put32(TX + 104 + 4, 0x710);
    putf(W + 16, 10); putf(W + 20, 20); putf(W + 24, 30); putf(W + 32, 8);
    putf(O + 408, 1); putf(O + 412, 2); putf(O + 416, 3);
    arena[O] = 2;
}
int main() {
    unsigned char* const o = arena + O;
    const Point3 p = {100, 200, 300};
    reset(5, 5, 0, 0.0f);  // frameNm is not below the end word: entry 8 with 3 through the table at +20
    run__18SpellDiseaseShieldG6Point3s(o, p, 0);
    if (calls != "entry2304(3);" || faces != 0) { std::puts(calls.c_str()); return 1; }
    // a quiet frame: growth 20 -> loops of (int)(20*0.00083*20*3.14) = 1, (int)(20*0.000417*20*3.14) = 0, sixteen faces
    reset(7, 100, 0, 20.0f);
    run__18SpellDiseaseShieldG6Point3s(o, p, 0);
    if (added.size() != 1 || faces != 16 || !calls.empty()) { std::printf("added %zu faces %d %s\\n", added.size(), faces, calls.c_str()); return 2; }
    if (added[0].def != static_cast<void*>(diseaseshield_run_particle_8d69b0)) return 3;
    // the caster's position is followed
    if (getf(O + 408) != 10.0f || getf(O + 412) != 20.0f || getf(O + 416) != 30.0f) return 4;
    // growth below the limit gains 0.5
    reset(7, 100, 40, 20.0f);
    run__18SpellDiseaseShieldG6Point3s(o, p, 0);
    if (getf(O + 428) != 20.5f) return 5;
    // loop two: growth 40 -> (int)(40*0.000417*40*3.14159) = 2 passes of the second record, and (int)(40*0.00083*40*3.14159) = 4 of the first
    reset(7, 100, 0, 40.0f);
    run__18SpellDiseaseShieldG6Point3s(o, p, 0);
    int ones = 0, twos = 0;
    for (const Added& a : added) { if (a.def == static_cast<void*>(diseaseshield_run_particle_8d69b0)) ++ones; else ++twos; }
    if (ones != 4 || twos != 2) { std::printf("ones %d twos %d\\n", ones, twos); return 6; }
    // a pulse frame with two listed objects, one refused
    reset(30, 100, 0, 0.0f); listed = {0x1000, 0x1400};
    run__18SpellDiseaseShieldG6Point3s(o, p, 0);
    if (calls != "box;w160;e104(0,2);w160;e104(0,2);lump;sound;") { std::puts(calls.c_str()); return 7; }
    if (attacks != 2 || attack_a != 40.0f || slot[0] != 38 || slot[1] != 40 || slot[2] != 40 || slot[3] != 0) return 8;
    if (attack_b != (40.0f + 40.0f * 0.75f * w13smb::bits(0x3e4cccccu)) * 0.5f) return 9;
    if (lump_a != "fx.lmp" || lump_b != "spel_de_dishield_hit.anm" || sound_at.x != 10.0f || sound_volume != 1.0f) return 10;
    // refused objects leave no sound
    reset(30, 100, 0, 0.0f); listed = {0x1000}; w160_result = 0;
    run__18SpellDiseaseShieldG6Point3s(o, p, 0);
    if (calls != "box;w160;" || attacks != 0) { std::puts(calls.c_str()); return 11; }
    // an empty list
    reset(30, 100, 0, 0.0f);
    run__18SpellDiseaseShieldG6Point3s(o, p, 0);
    if (calls != "box;") return 12;
    // other frames do not pulse
    reset(31, 100, 0, 0.0f);
    run__18SpellDiseaseShieldG6Point3s(o, p, 0);
    if (!calls.empty()) return 13;
    return 0;
}
