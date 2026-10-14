// Host contract for __13HammerOfWrathG6Point3sP10GameObjectiP5Skill; not an EE input. 32-bit words are offsets from a host arena;
// every callee appends a token to a call log and records the arguments that matter; the skill's table entry is routed to a logging
// stub. The authored function was also compared with the original in a private R5900 run (60 scenarios, every original word but one
// nop covered; mutants of the multiplier, the name table, the flag bit, the constants, the counter step and the description
// initialisation are caught there).
#define W14_PROJECTILE_A_HOST 1
#include "../w14-projectile-a-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w14_projectile_a_host_base;
void w14_projectile_a_host_hurt(unsigned, void*, float, void*, int) {}
static unsigned char arena[0x2000] __attribute__((aligned(16)));
static std::string calls;
static Point3 got_position;
static void *got_map, *link_head, *link_other, *link_item, *sound_object, *sound_header, *entry_object;
static std::string created_name, lump_a, lump_b;
static void* created_description; static int created_a, created_b;
static float sound_volume;
static unsigned entry_fn, entry_a1, entry_a2, entry_return;
static unsigned word164_at_create;
unsigned w14_projectile_a_host_call(unsigned fn, void* adjusted, int count, unsigned long a1, unsigned long a2) {
    calls += "entry;"; entry_fn = fn; entry_object = adjusted; entry_a1 = static_cast<unsigned>(a1); entry_a2 = static_cast<unsigned>(a2);
    return count == 2 ? entry_return : 0xbad;
}
extern "C" {
unsigned char wrathHammerCount;
unsigned char vt_HammerOfWrath[16] __asm__("_vt$13HammerOfWrath");
void game_object_construct(void* self, Point3 position, void* map, char** names) __asm__("__10GameObjectG6Point3PQ210GameObject11resourceMapPPc");
void game_object_construct(void* self, Point3 position, void* map, char** names) {
    calls += "ctor;"; got_position = position; got_map = map; if (names) calls += "BADNAMES;";
    std::memset(static_cast<unsigned char*>(self) + 12, 0, 4);
    std::memcpy(static_cast<unsigned char*>(self) + 16, &position, 12);
    std::memset(static_cast<unsigned char*>(self) + 76, 0x55, 12);
}
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* other, void* item) { calls += "link;"; link_head = head; link_other = other; link_item = item; }
void create__4itemPcPQ24item11descriptionii(const char* name, void* description, int a, int b) {
    calls += "create;"; created_name = name; created_description = description; created_a = a; created_b = b;
    word164_at_create = *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(description) + 76);
}
void objectAddToRunList__FP10GameObject(void*) { calls += "run;"; }
void* lumpFindResource__FPcT0(char* lump, char* name) { calls += "lump;"; lump_a = lump; lump_b = name; return arena + 0x800; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume) { calls += "sound;"; sound_header = header; sound_object = object; sound_volume = volume; }
}
#include "ctor.cpp"

static const char* const names[] = {"warhammer+frostburst10,10-20", "warhammer+frostburst12,12-24", "warhammer+frostburst14,14-26", "warhammer+frostburst16,16-28", "warhammer+frostburst18,18-30"};

static int run(void* creature, int n) {
    using namespace w14pa;
    std::memset(arena, 0xee, sizeof arena);
    calls.clear(); wrathHammerCount = 255;
    unsigned char* o = arena + 0x100;
    unsigned char* skill = arena + 0x400;
    word(skill, 96) = 0x600;
    word(arena + 0x600, 88 + 4) = 0xabc0u;
    *reinterpret_cast<short*>(arena + 0x600 + 88) = 16;
    entry_return = 7;
    Point3 p = {1.5f, -2.0f, 300.0f};
    hammer_of_wrath_construct(o, p, 0x18000, creature, n, skill);
    std::string want = std::string("ctor;") + (creature ? "link;" : "") + "create;run;lump;sound;entry;";
    if (calls != want) { std::puts(calls.c_str()); return 1; }
    if (got_position.x != 1.5f || got_position.z != 300.0f || got_map != nullptr) return 2;
    if (word(o, 72) != to_word(vt_HammerOfWrath) || half(o, 56) != -32768) return 3;
    if (word(o, 76) != 0 || word(o, 80) != 0) return 4;
    if (creature && (link_head != o + 76 || link_other != creature || link_item != creature)) return 5;
    const unsigned char* d = o + 88;
    for (int e = 0; e < 4; ++e) if (*reinterpret_cast<const short*>(d + 36 + 8 * e) != -1) return 6;
    if (d[32] != 0xff || d[33] != 0xff) return 7;
    for (int off = 16; off < 32; off += 2) if (*reinterpret_cast<const short*>(d + off) != 0) return 8;
    const char* want_name = (n >= 0 && n <= 3) ? names[n] : names[4];
    if (created_name != want_name || created_description != o + 88 || created_a != -1 || created_b != 0) return 9;
    if (word164_at_create != static_cast<unsigned>(n)) return 10;
    if (word(o, 164) != static_cast<unsigned>(n) + 1u || word(o, 12) != 1u) return 11;
    if (lump_a != "fx.lmp" || lump_b != "spel_he_hammer.anm" || sound_header != arena + 0x800 || sound_object != o || sound_volume != 1.0f) return 12;
    if (real(o, 180) != 16384.0f || real(o, 216) != 1.5f || real(o, 220) != -2.0f || real(o, 224) != 300.0f || word(o, 196) != to_word(skill)) return 13;
    if (entry_fn != 0xabc0u || entry_object != skill + 16 || entry_a1 != 1 || entry_a2 != static_cast<unsigned>(n) + 1u) return 14;
    if (word(o, 168) != 7u * 60u || wrathHammerCount != 0) return 15;
    return 0;
}
int main() {
    w14_projectile_a_host_base = arena;
    int r;
    for (int n : {0, 1, 2, 3, 4, -1, 99}) {
        if ((r = run(arena + 0x700, n))) return r + 20 * (n + 1);
        if ((r = run(nullptr, n))) return r + 20 * (n + 1) + 1000;
    }
    return 0;
}
