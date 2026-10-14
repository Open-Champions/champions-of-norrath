// Host contract for run__15SpellConeOfFireG6Point3s; not an EE input. 32-bit words are offsets from a host base and the callees log.
// The authored function was also compared with the original in a private R5900 run (random rolls with rejected draws, frame
// multiples of 15, caster counts, list contents, owner slot results and the angle; every original word covered).
#define W12_SPELL_C_HOST 1
#include "run.cpp"
#include <cmath>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w12_spell_c_host_base;
static std::string calls;
static unsigned yes_for;
unsigned w12_spell_c_host_call(unsigned fn, void*, int count, unsigned long a1, unsigned long) {
    calls += "slot" + std::to_string(fn) + "(" + std::to_string(count) + "," + std::to_string(a1) + ");";
    return fn == 0x70 && a1 == yes_for ? 1 : 0;
}
static int caster_n, caster_modes[8], caster_calls, caster_bytes[8];
static void* caster_args[8];
int w14b_host_call3(unsigned, void* adjusted, int first, int second, void* third) {
    calls += "amount" + std::to_string(first) + ";";
    caster_modes[caster_calls] = first; caster_bytes[caster_calls] = second; caster_args[caster_calls] = third; (void)adjusted;
    return caster_calls++ == 100 ? 0 : (first == 0 ? 20 : caster_n);
}
static std::vector<int> rolls;
static size_t rolls_used;
struct Added { Point3 where, velocity; void* def; };
static std::vector<Added> added;
static std::vector<int> cos_args, sin_args;
static std::vector<void*> nodes_to_make;
static Point3 list_center; static short list_angle; static float list_a, list_b;
struct Attack { void* target; void* source; int p, f; short slot[4]; };
static std::vector<Attack> attacks;
static std::string lump_b;
static Point3 sound_at; static float sound_volume;
static int destroyed;
extern "C" {
int frameNm;
unsigned char norrath_w13_spell_msg_a_view_8d7b30[80];
unsigned char norrath_w13_spell_msg_a_view_8d7b80[80];
unsigned char norrath_w13_spell_msg_a_view_8d7bd0[80];
int eRand__5Spell(void*) { return rolls[rolls_used++ % rolls.size()]; }
float icos__Fi(int a) { cos_args.push_back(a); return 1.0f; }
float isin__Fi(int a) { sin_args.push_back(a); return 0.5f; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float) { added.push_back({*p, *v, d}); }
void objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(Point3 center, short angle, float a, float b, void* list, void*) {
    calls += "list;"; list_center = center; list_angle = angle; list_a = a; list_b = b;
    unsigned char* h = static_cast<unsigned char*>(list);
    unsigned prev = 0;
    for (size_t i = nodes_to_make.size(); i-- > 0;) {
        unsigned char* n = w12_spell_c_host_base + 0x3000 + 16 * i;
        w12c::word(n, 0) = prev;
        w12c::word(n, 12) = w12c::to_word(nodes_to_make[i]);
        prev = w12c::to_word(n);
    }
    w12c::word(h, 0) = prev;
}
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void* b, float, float, int p, int, int, int f, void* d, bool) {
    calls += "attack;";
    Attack at{a, b, p, f, {}};
    std::memcpy(at.slot, static_cast<unsigned char*>(d) + 36, 8);
    attacks.push_back(at);
}
void* lumpFindResource__FPcT0(char*, char* b) { calls += "lump;"; lump_b = b; return nullptr; }
int soundPlaySequence__FP15AnimationHeaderG6Point3f(void*, Point3 p, float v) { calls += "sound;"; sound_at = p; sound_volume = v; return 0; }
void listDestroy__FP8ListHead(void*) { calls += "destroy;"; ++destroyed; }
}
static unsigned char arena[0x4000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static float getf(unsigned off) { float v; std::memcpy(&v, arena + off, 4); return v; }
enum { S = 0x100, CASTER = 0x800, OWNER = 0x900, OT = 0x980, E1 = 0xa00, E2 = 0xa80 };
static void reset(int frame, int n) {
    w12_spell_c_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    calls.clear(); added.clear(); attacks.clear(); cos_args.clear(); sin_args.clear(); nodes_to_make.clear();
    rolls = {49152}; rolls_used = 0; yes_for = 0xffffffffu; caster_n = n; caster_calls = 0; destroyed = 0;
    frameNm = frame;
    put32(S + 8, CASTER); put32(S + 12, OWNER); arena[S] = 5; put32(CASTER + 96, CASTER + 0x40);
    put32(OWNER + 72, OT); put32(OT + 160 + 4, 0x70);
    putf(OWNER + 16, 7); putf(OWNER + 20, 8); putf(OWNER + 24, 9);
}
static int count_def(void* def) { int n = 0; for (auto& a : added) if (a.def == def) ++n; return n; }
int main() {
    Point3 origin = {10.0f, 20.0f, 30.0f};
    // an off frame: the light is rewritten, 20 + 5 + 3 particles, no query
    reset(7, 2);
    run__15SpellConeOfFireG6Point3s(arena + S, &origin, 1000);
    if (getf(S + 52) != 90.0f || getf(S + 60) != 90.0f || std::fabs(getf(S + 56) - 1.0f / 8100.0f) > 1e-9 || std::fabs(getf(S + 64) - 1.0f / 8100.0f) > 1e-9) return 1;
    if (sin_args.size() < 2 || sin_args[0] != 7 * 3500 || sin_args[1] != 7 * 1750) return 2;
    if (caster_modes[0] != 1 || caster_bytes[0] != 5 || caster_args[0] != arena + OWNER) return 3;
    if (count_def(norrath_w13_spell_msg_a_view_8d7b30) != 20 || count_def(norrath_w13_spell_msg_a_view_8d7b80) != 5 || count_def(norrath_w13_spell_msg_a_view_8d7bd0) != 3 || added.size() != 28) return 4;
    // first particle: unit (.5) * 4.9998 + origin, velocity .5 + icos * 59.9998; the arc is angle - 91n + r * 182n
    if (std::fabs(added[0].where.x - (0.5f * 4.9998f + 10.0f)) > 1e-3 || std::fabs(added[0].velocity.x - (0.5f + 59.9998f)) > 1e-3 || std::fabs(added[0].velocity.y - (0.5f + 0.5f * 59.9998f)) > 1e-2) return 5;
    if (cos_args[0] != 1000 - 182 + static_cast<int>(0.75f * 364.0f) || cos_args[0] != 1091) return 6;
    if (added[20].where.x != 0.5f * 5.0f + 10.0f || std::fabs(added[25].where.z - (0.5f * 5.000007f + 30.0f)) > 1e-3) return 7;
    if (calls.find("list;") != std::string::npos) return 8;
    // a frame that is a multiple of 15: query centre is the origin less 12 * (icos, isin), the range is 96 and (m + 10)
    reset(15, 3);
    run__15SpellConeOfFireG6Point3s(arena + S, &origin, 1000);
    if (calls.find("list;") == std::string::npos || list_center.x != 10.0f - 12.0f || list_center.y != 20.0f - 6.0f || list_center.z != 30.0f || list_angle != 1000 || list_a != 96.0f || list_b != 13.0f) return 9;
    if (calls.find("sound") != std::string::npos || !attacks.empty() || destroyed != 1) return 10;
    // objects in the list: the owner's slot 160 chooses; a chosen one is hurt with (a, b) and the sound plays at the owner
    reset(30, 2);
    nodes_to_make = {arena + E1, arena + E2};
    yes_for = E2;
    run__15SpellConeOfFireG6Point3s(arena + S, &origin, 1000);
    if (attacks.size() != 1 || attacks[0].target != arena + E2 || attacks[0].source != arena + OWNER || attacks[0].p != 100000 || attacks[0].f != 64) return 11;
    // a = (int)(20 * 0.25) = 5; b = (int)((5 + (5 * 0.75) * 0.2) * 0.25) = 1
    if (attacks[0].slot[0] != 21 || attacks[0].slot[1] != 5 || attacks[0].slot[2] != 1 || attacks[0].slot[3] != 0) return 12;
    if (lump_b != "spel_ew_cfire_hit.anm" || sound_at.x != 7.0f || sound_at.z != 9.0f || sound_volume != 1.0f || destroyed != 1) return 13;
    // nobody chosen: no sound, but the list is still destroyed
    reset(30, 2);
    nodes_to_make = {arena + E1, arena + E2};
    run__15SpellConeOfFireG6Point3s(arena + S, &origin, 1000);
    if (calls.find("sound") != std::string::npos || !attacks.empty() || destroyed != 1) return 14;
    return 0;
}
