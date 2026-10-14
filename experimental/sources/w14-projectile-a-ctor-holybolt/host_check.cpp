// Host contract for __8HolyBoltG6Point3iiP8CreatureP5Skill; not an EE input. 32-bit words are offsets from a host arena; the box search,
// the table call, the trigonometry, the trail and every other callee are logging stubs, and the list it returns is set per scenario.
// The authored function was also compared with the original in a private R5900 run (100 scenarios, every original word but one nop
// covered; mutants of the reach, the initial distances, the dot-product sign, the type byte, the flag bit, the self and caster tests,
// the fallback to the back index, the frame factor and the trail and aim constants are caught there).
#define W14_PROJECTILE_A_HOST 1
#include "../w14-projectile-a-common/access.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w14_projectile_a_host_base;
void w14_projectile_a_host_hurt(unsigned, void*, float, void*, int) {}
static unsigned char arena[0x8000] __attribute__((aligned(16)));
static std::string calls;
static std::vector<unsigned> list_words;
static Point3 got_position, low_seen, high_seen, trail_tip;
static void* got_owner; static void* got_map;
static int list_capacity, skill_a1, skill_b;
static unsigned char light_at_add[64];
static int trail_args[8], trail_id_seen; static float trail_float;
static std::vector<int> trig_args;
static std::string lump_a, lump_b;
static void *sound_header, *sound_object; static float sound_volume;
unsigned w14_projectile_a_host_call(unsigned fn, void*, int count, unsigned long a1, unsigned long a2) {
    if (fn != 1 || count != 2) { calls += "BADFN;"; return 0; }
    calls += "skill;"; skill_a1 = static_cast<int>(a1); skill_b = static_cast<int>(a2);
    return 5;
}
static float table_cos(int angle) { return static_cast<float>(std::cos(angle * 2.0 * 3.14159265358979323846 / 65536.0)); }
static float table_sin(int angle) { return static_cast<float>(std::sin(angle * 2.0 * 3.14159265358979323846 / 65536.0)); }
extern "C" {
int frameNm;
unsigned char vt_HolyBolt[16] __asm__("_vt$8HolyBolt");
void projectile_construct(void* self, Point3 position, void* owner, void* map) __asm__("__10ProjectileG6Point3P10GameObjectPQ210GameObject11resourceMap");
void projectile_construct(void* self, Point3 position, void* owner, void* map) {
    calls += "ctor;"; got_position = position; got_owner = owner; got_map = map;
    std::memset(static_cast<unsigned char*>(self) + 12, 0, 4);
    std::memcpy(static_cast<unsigned char*>(self) + 16, &position, 12);
}
void lightAdd__FP5Light(void* light) { calls += "light;"; std::memcpy(light_at_add, light, sizeof light_at_add); }
float icos__Fi(int angle) { calls += "cos;"; trig_args.push_back(angle); return table_cos(angle); }
float isin__Fi(int angle) { calls += "sin;"; trig_args.push_back(angle); return table_sin(angle); }
void objectAddToRunList__FP10GameObject(void*) { calls += "run;"; }
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, unsigned* out, int capacity) {
    calls += "find;"; low_seen = low; high_seen = high; list_capacity = capacity;
    for (size_t i = 0; i < list_words.size(); ++i) out[i] = list_words[i];
    out[list_words.size()] = 0;
}
int P_StartTrail__Fiifiiiii(int a, int b, float c, int d, int e, int f, int g, int h) {
    calls += "trail;"; trail_args[0] = a; trail_args[1] = b; trail_args[2] = d; trail_args[3] = e; trail_args[4] = f; trail_args[5] = g; trail_args[6] = h; trail_float = c;
    return 41;
}
void P_ExtendTrail__FiG6Point3(int id, Point3 position) { calls += "extend;"; trail_id_seen = id; trail_tip = position; }
void* lumpFindResource__FPcT0(char* lump, char* name) { calls += "lump;"; lump_a = lump; lump_b = name; return arena + 0x800; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume) { calls += "sound;"; sound_header = header; sound_object = object; sound_volume = volume; }
}
#include "ctor.cpp"

static unsigned char* object(int i) { return arena + 0x1000 + 0x100 * i; }
static void make(int i, float x, float y, float z, unsigned flags, unsigned char kind) {
    using namespace w14pa;
    unsigned char* e = object(i);
    std::memset(e, 0, 0x100);
    real(e, 16) = x; real(e, 20) = y; real(e, 24) = z; word(e, 12) = flags; e[172] = kind;
}
static unsigned wordof(unsigned char* p) { return static_cast<unsigned>(p - arena); }

// variant 0: a front and a back candidate (front wins) followed by a farther front one that must be ignored; 1: only a back candidate (the caster follows it in the list); 2: no candidate; 3: empty list
static int run(int variant, int angle) {
    using namespace w14pa;
    std::memset(arena, 0xee, sizeof arena);
    calls.clear(); trig_args.clear(); frameNm = 500;
    unsigned char* o = arena + 0x100;
    unsigned char* caster = arena + 0x600;
    unsigned char* skill = arena + 0x800 + 0x400;
    word(skill, 96) = wordof(arena + 0xe00);
    *reinterpret_cast<short*>(arena + 0xe00 + 88) = 0;
    word(arena + 0xe00, 88 + 4) = 1;
    std::memset(caster, 0, 0x100);
    real(caster, 16) = 0.0f; real(caster, 20) = 0.0f; real(caster, 24) = 0.0f;
    half(caster, 56) = 0;
    make(2, 110.0f, 20.0f, 30.0f, 0x20, 16);   // front of the caster, distance 100 from the bolt
    make(3, -40.0f, 20.0f, 30.0f, 0x20, 16);   // behind the caster, distance 50
    make(4, 20.0f, 20.0f, 30.0f, 0x00, 16);    // flag 0x20 clear
    make(5, 20.0f, 20.0f, 30.0f, 0x28, 16);    // flag 0x08 set
    make(6, 20.0f, 20.0f, 30.0f, 0x20, 15);    // wrong type byte
    make(7, 5000.0f, 0.0f, 0.0f, 0x20, 16);    // too far for either best distance (starts at 10 million: still accepted when nothing else)
    make(8, 210.0f, 20.0f, 30.0f, 0x20, 16);   // in front, distance 200: farther than both best distances once the first two are found
    word(caster, 12) = 0x20; caster[172] = 16; // the caster itself would qualify if it were not skipped
    const unsigned self_word = wordof(o), caster_word = wordof(caster);
    if (variant == 0) list_words = {self_word, caster_word, wordof(object(4)), wordof(object(5)), wordof(object(6)), wordof(object(2)), wordof(object(3)), wordof(object(8))};
    if (variant == 1) list_words = {wordof(object(4)), wordof(object(3)), wordof(object(5)), wordof(object(6)), caster_word};
    if (variant == 2) list_words = {self_word, caster_word, wordof(object(4)), wordof(object(5)), wordof(object(6))};
    if (variant == 3) list_words = {};
    Point3 p = {10.0f, 20.0f, 30.0f};
    holy_bolt_construct(o, p, angle, 7, caster, skill);
    if (calls.find("BAD") != std::string::npos) { std::puts(calls.c_str()); return 1; }
    if (got_position.x != 10.0f || got_owner != caster || got_map != nullptr) return 2;
    if (word(o, 72) != to_word(vt_HolyBolt) || word(o, 12) != 0x4001u) return 3;
    const unsigned char* l = light_at_add;
    const short want_h[8] = {172, 252, 500, -32, 172, 252, 500, -32};
    const int offs[8] = {10, 12, 14, 16, 18, 20, 22, 24};
    if (l[8] != 0 || l[9] != 0) return 4;
    for (int i = 0; i < 8; ++i) if (*reinterpret_cast<const short*>(l + offs[i]) != want_h[i]) return 5;
    if (*reinterpret_cast<const float*>(l + 28) != 120.0f || *reinterpret_cast<const float*>(l + 36) != 120.0f || *reinterpret_cast<const unsigned*>(l + 32) != 0x3891a2b3u ||
        *reinterpret_cast<const unsigned*>(l + 40) != 0x3891a2b3u || *reinterpret_cast<const float*>(l + 44) != 0.0f) return 6;
    if (real(o, 164) != 10.0f || real(o, 168) != 20.0f || real(o, 172) != 30.0f) return 7;
    const short a = static_cast<short>(angle);
    if (half(o, 56) != a || real(o, 28) != 18.0f) return 8;
    if (real(o, 88) != table_cos(a) * 350.0f || real(o, 92) != table_sin(a) * 350.0f || real(o, 96) != 0.0f) return 9;
    if (word(o, 184) != wordof(skill) || word(o, 188) != 7u || skill_a1 != 2 || skill_b != 7 || word(o, 84) != 500u + 5u * 60u) return 10;
    if (low_seen.x != 10.0f - 360.0f || high_seen.z != 30.0f + 360.0f || list_capacity != 32) return 11;
    unsigned target = 0;
    if (variant == 0) target = wordof(object(2));
    if (variant == 1) target = wordof(object(3));
    if (word(o, 100) != target) return 12;
    if (variant >= 2) {
        const float aim = 12.0f, far = 4000.0f;
        if (real(o, 104) != 10.0f + table_cos(0) * aim * far || real(o, 108) != 20.0f + table_sin(0) * aim * far || real(o, 112) != 30.0f + 36.0f) return 13;
    } else if (real(o, 104) != 0.0f && std::memcmp(o + 104, "\xee\xee\xee\xee", 4) != 0) return 14;  // untouched
    if (trail_args[0] != 4 || trail_args[1] != 750 || trail_args[2] != 4 || trail_args[3] != 107 || trail_args[4] != 100 || trail_args[5] != 234 || trail_args[6] != 171) return 15;
    if (trail_float != bits(0x3ea4dd2fu) || word(o, 180) != 41u || trail_id_seen != 41) return 16;
    if (trail_tip.x != 10.0f + table_cos(a) * 10.0f || trail_tip.y != 20.0f + table_sin(a) * 10.0f || trail_tip.z != 30.0f) return 17;
    if (lump_a != "fx.lmp" || lump_b != "spel_he_dismiss.anm" || sound_header != arena + 0x800 || sound_object != o || sound_volume != 1.0f) return 18;
    if (calls.substr(0, 34) != "ctor;light;cos;sin;skill;run;find;") return 19;
    return 0;
}
int main() {
    w14_projectile_a_host_base = arena;
    int r;
    for (int v = 0; v < 4; ++v)
        for (int ang : {0, 0x4000, -0x4000, 0x12345}) if ((r = run(v, ang))) return r + 20 * (v + 1);
    return 0;
}
