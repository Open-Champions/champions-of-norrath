// Observed cast__18SkillHammerOfWrathiP8CreatureG6Point3s at 0x002b50c8, 644 bytes in .text.
// Steps, in order (skill = first argument, rank = second, creature = third, point = the by-value Point3 copied to
// the frame, angle = the trailing short):
//  1. eight halfwords of the creature are written: -100 at byte offsets 140, 142, 144, 148, 150, 152 and zero at 146
//     and 154;
//  2. 92 bytes are allocated with blockAlloc__Fi and, when the block is not null,
//     __12GlowingShaftG6Point3iiii is called with (block, point, 90, 86, 126, 250);
//  3. 188 bytes are allocated and, when the block is not null, __11LightEffectG6Point3ffffffffffffP10GameObjecti is called
//     with (block, the point with 36.0 added to its third float and 0.0 added to its first, then the twelve floats
//     0, 155, 255, -96, 0, 0, 0, 0, 180, 144, 0, 1, a null object and the integer 0);
//  4. 232 bytes are allocated and, when the block H is not null, __13HammerOfWrathG6Point3sP10GameObjectiP5Skill is
//     called with (H, the point with 60.0 added to its third float and 0.0 added to its first, angle, creature, rank,
//     skill); when the block is null H is treated as null;
//  5. when H is null the function returns null. Otherwise 28 bytes are allocated; when that block O is null the function
//     returns null. Otherwise O is filled in place and returned: byte 0 = the low byte of rank; word +4 = 0; word +8 =
//     skill; word +12 = creature; syncRandState is replaced by syncRandState * 0x19660d + 0x3c6ef35f and word +16 =
//     the new value shifted right by 16; word +20 = the address of the table _vt$5Spell and then, finally, of the table
//     _vt$18SpellHammerOfWrath; word +24 = 0; and listOnewayLink__FP8ListHeadT0Pv is called with (O + 24, H, H).
// Inferred: step 5 is an inlined constructor of a spell object for this skill (the two table addresses and the shared
// first fields match the spell base class), and the object ties itself to the projectile H through the list link. The 28
// bytes, the multiplier and the increment are as observed; the generator is the shared sync random state.
// Unknown: the meaning of the point offsets and of the many float and integer constants, the colour group of the
// creature, the list link semantics, and the layouts of skill and creature. Nothing here ran on the EE.
#include "../w13-skill-cast-b-common/skill_access.hpp"

extern "C" {
void* blockAlloc__Fi(int size);
void glowing_shaft_ctor(void* self, Point3 point, int a, int b, int c, int d) asm("__12GlowingShaftG6Point3iiii");
void light_effect_ctor(void* self, Point3 point, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12,
                       void* object, int mode) asm("__11LightEffectG6Point3ffffffffffffP10GameObjecti");
void hammer_ctor(void* self, Point3 point, short angle, void* creature, int rank, void* skill) asm("__13HammerOfWrathG6Point3sP10GameObjectiP5Skill");
void listOnewayLink__FP8ListHeadT0Pv(void* list, void* node, void* data);
extern unsigned syncRandState;
extern char vt_spell_base[] asm("_vt$5Spell");
extern char vt_spell_hammer_of_wrath[] asm("_vt$18SpellHammerOfWrath");
void* cast_hammer_of_wrath(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__18SkillHammerOfWrathiP8CreatureG6Point3s");
}
static inline __attribute__((always_inline)) unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
extern "C" void* cast_hammer_of_wrath(void* skill, int rank, void* creature, Point3 point, short angle) {
    short* colours = reinterpret_cast<short*>(static_cast<unsigned char*>(creature) + 140);
    colours[2] = -100;
    colours[4] = -100;
    colours[0] = -100;
    colours[5] = -100;
    colours[1] = -100;
    colours[6] = -100;
    colours[7] = 0;
    colours[3] = 0;
    void* shaft = blockAlloc__Fi(92);
    if (shaft) glowing_shaft_ctor(shaft, point, 90, 86, 126, 250);
    void* light = blockAlloc__Fi(188);
    if (light) {
        Point3 at;
        at.x = point.x + 0.0f;
        at.y = point.y;
        at.z = point.z + 36.0f;
        light_effect_ctor(light, at, 0.0f, 155.0f, 255.0f, -96.0f, 0.0f, 0.0f, 0.0f, 0.0f, 180.0f, 144.0f, 0.0f, 1.0f, nullptr, 0);
    }
    void* hammer = blockAlloc__Fi(232);
    if (hammer) {
        Point3 at;
        at.x = point.x + 0.0f;
        at.y = point.y;
        at.z = point.z + 60.0f;
        hammer_ctor(hammer, at, angle, creature, rank, skill);
    }
    if (!hammer) return nullptr;
    unsigned char* object = static_cast<unsigned char*>(blockAlloc__Fi(28));
    if (!object) return nullptr;
    object[0] = static_cast<unsigned char>(rank);
    syncRandState = syncRandState * 0x19660du + 0x3c6ef35fu;
    *reinterpret_cast<unsigned*>(object + 20) = word_of(vt_spell_base);
    *reinterpret_cast<unsigned*>(object + 4) = 0;
    *reinterpret_cast<unsigned*>(object + 12) = word_of(creature);
    *reinterpret_cast<unsigned*>(object + 8) = word_of(skill);
    *reinterpret_cast<unsigned*>(object + 16) = syncRandState >> 16;
    *reinterpret_cast<unsigned*>(object + 20) = word_of(vt_spell_hammer_of_wrath);
    *reinterpret_cast<unsigned*>(object + 24) = 0;
    listOnewayLink__FP8ListHeadT0Pv(object + 24, hammer, hammer);
    return object;
}
