// Observed __19BlindingLightEffectG6Point3iP8CreatureP5Skill at 0x002b94d0, 916 bytes. Arguments: the object, a position P (by value), an
// integer N, a caster creature C and a skill K. Returns nothing. Table calls below are (adjusted object, arguments...) through the
// signed halfword of the entry. In order:
//  1. The GameObject constructor runs with (object, P, null, null). _vt$19BlindingLightEffect goes to +72, frameNm + 60 to +76 and
//     bit mask 0x421b is set in the flag word at +12.
//  2. The entry at byte 88 of the table at the word K+96 is called on K with (3, N), giving R. The word +80 becomes 12 * R and the
//     object's position (the singles at +16, +20, +24) plus and minus float(12 * R) gives a box, handed to objectFindInBox with a
//     64-word list on the frame and the capacity 64.
//  3. For each listed object T up to the first zero word: the entry at byte 160 of C's table (word +72) is called on C with T; a zero
//     result skips T. Otherwise the entry at byte 88 of K's table is called with (0, N), giving S; when isBoss(T) is nonzero S becomes
//     S / 2 + 1 (signed, rounding toward zero). With e = eRand__10GameObject(object) the test float(int(e)) * 2^-16 * 100.0 < float(S)
//     must hold (an unsigned-looking roll is read as signed). If it holds: blockAlloc(32) and, when non-null,
//     __18SpellBlindingLightiP8CreatureT2P5Skill(block, N, C, T, K); addSpell__8CreatureP5Spell(T, block) runs (with a null block too);
//     the entry at byte 88 of K's table is called with (2, N), giving D, and the entry at byte 216 of T's table is called on T with
//     60 * D.
//  4. blockAlloc(188) and, when non-null, __11LightEffectG6Point3ffffffffffffP10GameObjecti(block, the object's position, then the
//     twelve singles 86.0, 128.0, 250.0, -32.0, 0, 0, 0, 0, float(W + 24), float(W / 2 + 24) (signed, rounding toward zero),
//     0.6 (the single 0x3f199999) and 1.0, where W is the word at +80, then a null object and 0).
//  5. blockAlloc(92) and, when non-null, __12GlowingShaftG6Point3iiii(block, the object's position, 60, 86, 128, 250).
//     objectAddToRunList runs on the object.
// Inferred: the byte 160 entry of the caster is an enemy test (see the combat-reactions notes), the 12 * R radius and the roll against a
// skill-given percentage give a chance per enemy to receive the blinding spell, bosses getting half the chance plus one. Unknown: what
// N is (the cast path passes the skill rank), the meaning of the entries at 88 and 216, and the light constants. Nothing here ran on
// the EE.
#include "../w14-projectile-a-common/access.hpp"

extern "C" {
extern int frameNm;
extern unsigned char vt_BlindingLightEffect[] __asm__("_vt$19BlindingLightEffect");
void game_object_construct(void* self, Point3 position, void* map, char** names) __asm__("__10GameObjectG6Point3PQ210GameObject11resourceMapPPc");
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, unsigned* out, int capacity);
int isBoss__FP10GameObject(void* object);
unsigned eRand__10GameObject(void* object);
void* blockAlloc__Fi(int size);
void spell_blinding_light_construct(void* self, int n, void* caster, void* target, void* skill) __asm__("__18SpellBlindingLightiP8CreatureT2P5Skill");
void addSpell__8CreatureP5Spell(void* creature, void* spell);
void light_effect_ctor(void* self, Point3 point, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12,
                       void* object, int mode) asm("__11LightEffectG6Point3ffffffffffffP10GameObjecti");
void glowing_shaft_ctor(void* self, Point3 point, int a, int b, int c, int d) asm("__12GlowingShaftG6Point3iiii");
void objectAddToRunList__FP10GameObject(void* object);
void blinding_light_effect_construct(void* self, Point3 position, int n, void* caster, void* skill) __asm__("__19BlindingLightEffectG6Point3iP8CreatureP5Skill");
}

extern "C" void blinding_light_effect_construct(void* self, Point3 position, int n, void* caster, void* skill) {
    using namespace w14pa;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const k = static_cast<unsigned char*>(skill);
    unsigned char* const c = static_cast<unsigned char*>(caster);
    game_object_construct(self, position, nullptr, nullptr);
    word(o, 72) = to_word(vt_BlindingLightEffect);
    word(o, 76) = static_cast<unsigned>(frameNm) + 60u;
    word(o, 12) |= 0x421bu;
    const int r = static_cast<int>(call_entry_from(k, 96, 88, 3, static_cast<unsigned>(n)));
    const int radius_words = r * 12;
    word(o, 80) = static_cast<unsigned>(radius_words);
    const float radius = static_cast<float>(radius_words);
    const float x = real(o, 16), y = real(o, 20), z = real(o, 24);
    Point3 low, high;
    low.x = x - radius;
    low.y = y - radius;
    low.z = z - radius;
    high.x = x + radius;
    high.y = y + radius;
    high.z = z + radius;
    unsigned list[64];
    objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, list, 64);
    for (int i = 0; list[i] != 0; ++i) {
        unsigned char* const target = from_word(list[i]);
        if (call_entry(c, 160, to_word(target)) == 0) continue;
        int s = static_cast<int>(call_entry_from(k, 96, 88, 0, static_cast<unsigned>(n)));
        if (isBoss__FP10GameObject(target) != 0) s = s / 2 + 1;
        const float roll = static_cast<float>(static_cast<int>(eRand__10GameObject(self))) * bits(0x37800000u) * bits(0x42c80000u);
        if (!(roll < static_cast<float>(s))) continue;
        void* const block = blockAlloc__Fi(32);
        if (block != nullptr) spell_blinding_light_construct(block, n, caster, target, skill);
        addSpell__8CreatureP5Spell(target, block);
        const int d = static_cast<int>(call_entry_from(k, 96, 88, 2, static_cast<unsigned>(n)));
        call_entry(target, 216, static_cast<unsigned long>(static_cast<unsigned>(d * 60)));
    }
    void* const light = blockAlloc__Fi(188);
    if (light != nullptr) {
        const int w = static_cast<int>(word(o, 80));
        light_effect_ctor(light, *reinterpret_cast<Point3*>(o + 16), bits(0x42ac0000u), bits(0x43000000u), bits(0x437a0000u), bits(0xc2000000u), 0.0f, 0.0f, 0.0f, 0.0f,
                          static_cast<float>(w + 24), static_cast<float>(w / 2 + 24), bits(0x3f199999u), 1.0f, nullptr, 0);
    }
    void* const shaft = blockAlloc__Fi(92);
    if (shaft != nullptr) glowing_shaft_ctor(shaft, *reinterpret_cast<Point3*>(o + 16), 60, 86, 128, 250);
    objectAddToRunList__FP10GameObject(self);
}
