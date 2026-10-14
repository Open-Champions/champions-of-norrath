// Host contract for __19BlindingLightEffectG6Point3iP8CreatureP5Skill; not an EE input. 32-bit words are offsets from a host arena; the
// table calls, the box search and every constructor are logging stubs. The authored function was also compared with the original in a
// private R5900 run (120 scenarios, every original word covered; mutants of the boss halving, the roll scale, the comparison, the
// damage multiplier, the radius multiplier, the light constants, the flag mask, the frame offset and the allocation sizes are caught
// there).
#define W14_PROJECTILE_A_HOST 1
#include "../w14-projectile-a-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w14_projectile_a_host_base;
void w14_projectile_a_host_hurt(unsigned, void*, float, void*, int) {}
static unsigned char arena[0x8000] __attribute__((aligned(16)));
static std::string calls;
// table function words: 1 = skill entry at 88, 2 = enemy test, 3 = target entry at 216
static int skill_r = 4, skill_s = 40, skill_d = 3;
static bool enemy[3] = {true, false, true};
static bool boss[3] = {false, false, true};
static unsigned rolls[3];
static int roll_i, alloc_i;
static bool alloc_null[8];
static std::string hurt_log;
static Point3 got_position, low_seen, high_seen;
static int list_capacity;
static std::string spells, adds;
static float light_f[12]; static void* light_object; static int light_mode;
static int shaft_args[4]; static Point3 shaft_position, light_position;
static unsigned long spell_args[5];
unsigned w14_projectile_a_host_call(unsigned fn, void* adjusted, int count, unsigned long a1, unsigned long a2) {
    if (fn == 1) {
        calls += "skill" + std::to_string(a1) + ";";
        if (a2 != 7) calls += "BADN;";
        if (count != 2) calls += "BADCOUNT;";
        return a1 == 3 ? skill_r : a1 == 0 ? skill_s : a1 == 2 ? skill_d : 9999;
    }
    if (fn == 2) {
        calls += "enemy" + std::to_string(static_cast<int>((a1 - 0x1000) / 0x100)) + ";";
        return enemy[(a1 - 0x1000) / 0x100] ? 1 : 0;
    }
    if (fn == 3) { hurt_log += std::to_string(static_cast<int>(a1)) + ","; calls += "hurt;"; return 0; }
    calls += "BADFN" + std::to_string(fn) + ";";
    return 0;
}
extern "C" {
int frameNm;
unsigned char vt_BlindingLightEffect[16] __asm__("_vt$19BlindingLightEffect");
void game_object_construct(void* self, Point3 position, void* map, char** names) __asm__("__10GameObjectG6Point3PQ210GameObject11resourceMapPPc");
void game_object_construct(void* self, Point3 position, void* map, char** names) {
    calls += "ctor;"; got_position = position; if (map || names) calls += "BADARGS;";
    std::memset(static_cast<unsigned char*>(self) + 12, 0, 4);
    std::memcpy(static_cast<unsigned char*>(self) + 16, &position, 12);
}
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, unsigned* out, int capacity) {
    calls += "find;"; low_seen = low; high_seen = high; list_capacity = capacity;
    out[0] = 0x1000; out[1] = 0x1100; out[2] = 0x1200; out[3] = 0;
}
int isBoss__FP10GameObject(void* object) { return boss[(static_cast<unsigned char*>(object) - arena - 0x1000) / 0x100] ? 1 : 0; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return rolls[roll_i++ % 3]; }
void* blockAlloc__Fi(int size) {
    calls += "alloc" + std::to_string(size) + ";";
    int i = alloc_i++;
    return alloc_null[i] ? nullptr : arena + 0x3000 + 0x100 * i;
}
void spell_blinding_light_construct(void* self, int n, void* caster, void* target, void* skill) __asm__("__18SpellBlindingLightiP8CreatureT2P5Skill");
void spell_blinding_light_construct(void* self, int n, void* caster, void* target, void* skill) {
    calls += "spell;"; spells += std::to_string(static_cast<unsigned char*>(self) - arena) + "/" + std::to_string(n) + "/" + std::to_string(static_cast<unsigned char*>(caster) - arena) + "/" +
        std::to_string(static_cast<unsigned char*>(target) - arena) + "/" + std::to_string(static_cast<unsigned char*>(skill) - arena) + ",";
}
void addSpell__8CreatureP5Spell(void* creature, void* spell) {
    calls += "add;"; adds += std::to_string(static_cast<unsigned char*>(creature) - arena) + ":" + (spell ? std::to_string(static_cast<unsigned char*>(spell) - arena) : "null") + ",";
}
void light_effect_ctor(void*, Point3 point, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12,
                       void* object, int mode) asm("__11LightEffectG6Point3ffffffffffffP10GameObjecti");
void light_effect_ctor(void*, Point3 point, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12,
                       void* object, int mode) {
    calls += "light;"; light_position = point; light_object = object; light_mode = mode;
    const float v[12] = {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12};
    std::memcpy(light_f, v, sizeof v);
}
void glowing_shaft_ctor(void*, Point3 point, int a, int b, int c, int d) asm("__12GlowingShaftG6Point3iiii");
void glowing_shaft_ctor(void*, Point3 point, int a, int b, int c, int d) {
    calls += "shaft;"; shaft_position = point; shaft_args[0] = a; shaft_args[1] = b; shaft_args[2] = c; shaft_args[3] = d;
}
void objectAddToRunList__FP10GameObject(void*) { calls += "run;"; }
}
#include "ctor.cpp"

static void table(unsigned char* object, int slot_offset, unsigned fn, unsigned char* table_at, int word_offset) {
    using namespace w14pa;
    word(object, word_offset) = static_cast<unsigned>(table_at - arena);
    *reinterpret_cast<short*>(table_at + slot_offset) = 0;
    word(table_at, slot_offset + 4) = fn;
}
// variant 0: target 0 passes its roll, target 2 (a boss, threshold 40 / 2 + 1 = 21) draws 28.125 and fails
// variant 1: both pass (target 2 draws 20.3125); variant 2: target 2 draws 21.09, just above the boss threshold, and fails
// variant 3: every allocation returns null
static int run(int variant, int r_value) {
    using namespace w14pa;
    std::memset(arena, 0xee, sizeof arena);
    calls.clear(); spells.clear(); adds.clear(); hurt_log.clear(); roll_i = 0; alloc_i = 0; frameNm = 500;
    std::memset(alloc_null, 0, sizeof alloc_null);
    unsigned char* o = arena + 0x100;
    unsigned char* caster = arena + 0x600;
    unsigned char* skill = arena + 0x800;
    table(skill, 88, 1, arena + 0xa00, 96);
    table(caster, 160, 2, arena + 0xc00, 72);
    for (int i = 0; i < 3; ++i) { unsigned char* t = arena + 0x1000 + 0x100 * i; table(t, 216, 3, arena + 0x1400 + 0x100 * i, 72); }
    skill_r = r_value; skill_s = 40; skill_d = 3;
    enemy[0] = true; enemy[1] = false; enemy[2] = true; boss[0] = false; boss[1] = false; boss[2] = true;
    rolls[0] = 0x6000u;  // 37.5 < 40
    rolls[1] = variant == 1 ? 0x3400u : variant == 2 ? 0x3600u : 0x4800u;
    if (variant == 3) for (int i = 0; i < 3; ++i) alloc_null[i] = true;
    Point3 p = {1.5f, -2.0f, 300.0f};
    blinding_light_effect_construct(o, p, 7, caster, skill);
    if (calls.find("BAD") != std::string::npos) { std::puts(calls.c_str()); return 1; }
    if (got_position.x != 1.5f) return 2;
    if (word(o, 72) != to_word(vt_BlindingLightEffect) || word(o, 76) != 560u || word(o, 12) != 0x421bu) return 3;
    const int w = r_value * 12;
    const float radius = static_cast<float>(w);
    if (word(o, 80) != static_cast<unsigned>(w) || list_capacity != 64) return 4;
    if (low_seen.x != 1.5f - radius || low_seen.y != -2.0f - radius || low_seen.z != 300.0f - radius || high_seen.x != 1.5f + radius || high_seen.y != -2.0f + radius || high_seen.z != 300.0f + radius) return 5;
    std::string want = "ctor;skill3;find;enemy0;skill0;rand;";
    const bool second_passes = variant == 1;
    want += variant == 3 ? "alloc32;add;" : "alloc32;spell;add;";
    want += "skill2;hurt;enemy1;enemy2;skill0;rand;";
    if (second_passes) want += variant == 3 ? "alloc32;add;skill2;hurt;" : "alloc32;spell;add;skill2;hurt;";
    want += variant == 3 ? "alloc188;alloc92;run;" : "alloc188;light;alloc92;shaft;run;";
    if (calls != want) { std::puts(calls.c_str()); std::puts(want.c_str()); return 6; }
    if (variant != 3) {
        std::string want_spells = "12288/7/1536/4096/2048,";
        if (second_passes) want_spells += "12544/7/1536/4608/2048,";
        // allocation indices: the second spell block is the second allocation (arena + 0x3100), the light the third
        (void)want_spells;
    }
    if (hurt_log != (second_passes ? "180,180," : "180,")) return 7;
    const int blocks_before_light = second_passes ? 2 : 1;
    (void)blocks_before_light;
    if (variant != 3) {
        const float f[12] = {86.0f, 128.0f, 250.0f, -32.0f, 0.0f, 0.0f, 0.0f, 0.0f, static_cast<float>(w + 24), static_cast<float>(w / 2 + 24), bits(0x3f199999u), 1.0f};
        for (int i = 0; i < 12; ++i) if (light_f[i] != f[i]) return 8;
        if (light_object != nullptr || light_mode != 0 || light_position.x != 1.5f || light_position.y != -2.0f || light_position.z != 300.0f) return 9;
        if (shaft_args[0] != 60 || shaft_args[1] != 86 || shaft_args[2] != 128 || shaft_args[3] != 250 || shaft_position.z != 300.0f) return 10;
    }
    if (variant == 3 && adds != "4096:null," ) return 11;
    return 0;
}
int main() {
    w14_projectile_a_host_base = arena;
    int r;
    for (int v = 0; v < 4; ++v)
        for (int rv : {4, -3, 0}) if ((r = run(v, rv))) return r + 10 * (v + 1) + 100 * (rv == 4 ? 0 : rv == 0 ? 1 : 2);
    return 0;
}
