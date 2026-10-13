// Host contract for __10HolyStrikesiG6Point3P10GameObjectP5Skill; not an EE input. 32-bit words are offsets from a host arena; every
// callee appends a token to a call log and records the arguments that matter. The authored function was also compared with the
// original in a private R5900 run (48 scenarios, every original word covered; mutants of the frame offsets, the reach factor, the
// trail numbers, the cone numbers, the owner flag test and the halfword offsets are caught there).
#define W14_PROJECTILE_A_HOST 1
#include "../w14-projectile-a-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w14_projectile_a_host_base;
unsigned w14_projectile_a_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
void w14_projectile_a_host_hurt(unsigned, void*, float, void*, int) {}
static unsigned char arena[0x2000] __attribute__((aligned(16)));
static std::string calls;
static Point3 got_position, trail_position, list_position;
static void *got_owner, *got_map, *link_head, *link_other, *link_item, *list_head, *list_creature, *sound_object, *sound_header;
static int trail_args[8], trail_id_seen, angle_for_trig[2], list_angle, trig_n;
static float trail_float, list_a, list_b, sound_volume;
static std::string lump_a, lump_b;
extern "C" {
int frameNm;
unsigned char vt_HolyStrike[16] __asm__("_vt$10HolyStrike");
void projectile_construct(void* self, Point3 position, void* owner, void* map) __asm__("__10ProjectileG6Point3P10GameObjectPQ210GameObject11resourceMap");
void projectile_construct(void* self, Point3 position, void* owner, void* map) {
    calls += "ctor;"; got_position = position; got_owner = owner; got_map = map;
    std::memset(static_cast<unsigned char*>(self) + 12, 0, 4);
    std::memcpy(static_cast<unsigned char*>(self) + 16, &position, 12);
    std::memset(static_cast<unsigned char*>(self) + 84, 0x55, 8);
}
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* other, void* item) { calls += "link;"; link_head = head; link_other = other; link_item = item; }
float icos__Fi(int angle) { calls += "cos;"; angle_for_trig[0] = angle; return 2.0f; }
float isin__Fi(int angle) { calls += "sin;"; angle_for_trig[1] = angle; return -3.0f; }
int P_StartTrail__Fiifiiiii(int a, int b, float c, int d, int e, int f, int g, int h) {
    calls += "trail;"; trail_args[0] = a; trail_args[1] = b; trail_args[2] = d; trail_args[3] = e; trail_args[4] = f; trail_args[5] = g; trail_args[6] = h; trail_float = c;
    return 41;
}
void P_ExtendTrail__FiG6Point3(int id, Point3 position) { calls += "extend;"; trail_id_seen = id; trail_position = position; }
void objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(Point3 position, short angle, float a, float b, void* list, void* creature) {
    calls += "sorted;"; list_position = position; list_angle = angle; list_a = a; list_b = b; list_head = list; list_creature = creature;
}
void objectAddToRunList__FP10GameObject(void*) { calls += "run;"; }
void* lumpFindResource__FPcT0(char* lump, char* name) { calls += "lump;"; lump_a = lump; lump_b = name; return arena + 0x800; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume) { calls += "sound;"; sound_header = header; sound_object = object; sound_volume = volume; }
}
#include "ctor.cpp"

static int run(void* owner, unsigned owner_flags, bool expect_creature) {
    using namespace w14pa;
    std::memset(arena, 0xee, sizeof arena);
    calls.clear(); frameNm = 1000;
    unsigned char* o = arena + 0x100;
    if (owner) word(static_cast<unsigned char*>(owner), 12) = owner_flags;
    Point3 p = {1.5f, -2.0f, 300.0f};
    unsigned char* skill = arena + 0x400;
    holy_strike_construct(o, 0x18000, 0x12345, p, owner, skill);  // angle 0x18000 -> short -32768
    std::string want = std::string("ctor;") + (owner ? "link;" : "") + "cos;sin;trail;extend;sorted;run;lump;sound;";
    if (calls != want) { std::puts(calls.c_str()); return 1; }
    if (got_position.x != 1.5f || got_position.z != 300.0f || got_owner != owner || got_map != nullptr) return 2;
    if (word(o, 72) != to_word(vt_HolyStrike) || half(o, 56) != -32768) return 3;
    if (word(o, 84) != 0 || word(o, 88) != 0) return 4;
    if (owner && (link_head != o + 84 || link_other != owner || link_item != owner)) return 5;
    if (word(o, 100) != 1015u || word(o, 96) != 1180u || half(o, 92) != 0x2345 || word(o, 104) != to_word(skill)) return 6;
    if (angle_for_trig[0] != -32768 || angle_for_trig[1] != -32768) return 7;
    if (real(o, 108) != 2.0f * 450.0f || real(o, 112) != -3.0f * 450.0f || word(o, 116) != 0 || word(o, 120) != 0) return 8;
    if (trail_args[0] != 1 || trail_args[1] != 250 || trail_args[2] != 0 || trail_args[3] != 250 || trail_args[4] != 200 || trail_args[5] != 100 || trail_args[6] != 128) return 9;
    if (trail_float != bits(0x3e4ccccc) || word(o, 124) != 41u || trail_id_seen != 41) return 10;
    if (trail_position.x != 1.5f || trail_position.y != -2.0f || trail_position.z != 300.0f) return 11;
    if (list_position.x != 1.5f || list_position.z != 300.0f || list_angle != -32768 || list_a != 360.0f || list_b != 75.0f || list_head != o + 88) return 12;
    if (list_creature != (expect_creature ? owner : nullptr)) return 13;
    if (lump_a != "fx.lmp" || lump_b != "spel_he_hstrike.anm" || sound_header != arena + 0x800 || sound_object != o || sound_volume != 1.0f) return 14;
    return 0;
}
int main() {
    w14_projectile_a_host_base = arena;
    int r;
    if ((r = run(arena + 0x600, 0x20, true))) return r;
    if ((r = run(arena + 0x600, 0x40, false))) return 20 + r;
    if ((r = run(arena + 0x600, 0x21, true))) return 40 + r;
    return (r = run(nullptr, 0, false)) ? 60 + r : 0;
}
