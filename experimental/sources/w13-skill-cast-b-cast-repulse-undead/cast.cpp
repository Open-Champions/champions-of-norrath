// Observed cast__18SkillRepulseUndeadiP8CreatureG6Point3s at 0x002ba1d8, 732 bytes in .text.
// Steps, in order (skill = first argument, rank = second, creature = third, point = the by-value Point3 copied to the
// frame; the trailing short is not read; the function returns zero in every case):
//  1. a ring of objects: with the integer angle starting at 0, and repeating while the angle is at most 65535: a
//     velocity-like Point3 (icos__Fi(angle) * 450.0, isin__Fi(angle) * 450.0, 50.0) and a position (the point's first float
//     plus 0.0, its second float, its third float plus 36.0) are built; 176 bytes are allocated with blockAlloc__Fi; when
//     the block is not null __8LostSoulG6Point3T1iP10_vagHeaderii is called with (block, position, velocity, angle, null,
//     1, 0); then the angle is increased by the signed quotient 0x10000 / (rank + 4);
//  2. 188 bytes are allocated and, when the block is not null, __11LightEffectG6Point3ffffffffffffP10GameObjecti is called
//     with (block, the point with 36.0 added to its third float and 0.0 added to its first, then the twelve floats 100,
//     100, 255, -96, 0, 0, 0, 0, 120, 360, 0, 1, a null object and the integer 0);
//  3. the skill's table (the word at skill+96) entry at byte 88 (slot 11, evaluateRampAtRank__5Skillii in the image) is
//     called as (skill + adjustment, 0, rank), giving A, and again as (skill + adjustment, 1, rank), giving B;
//  4. gameRadialDamage__FP10GameObjectG6Point3iffiii is called with (creature, the point, A, 12 * B as a float, 24 * B as a
//     float, 0x0a100900, 64, 0);
//  5. the resource "spel_he_repulse.anm" is looked up in "fx.lmp" and played at the point with
//     soundPlaySequence__FP15AnimationHeaderG6Point3f(result, point, 1.0).
// Inferred: this spawns a ring of lost-soul objects around the point, a light, and a radial damage whose size comes
// from the skill's ramp. The count of ring objects is 65536 / (rank + 4) rounded up. The division by (rank + 4) is
// the signed machine division; a rank of -4 or a rank above 65532 does not end the ring loop sensibly and is not
// exercised.
// Unknown: the meaning of the many constants, of the 0x0a100900 value, of the damage arguments, and the layouts of skill
// and creature. Nothing here ran on the EE.
#include "../w13-skill-cast-b-common/skill_access.hpp"

extern "C" {
float icos__Fi(int angle);
float isin__Fi(int angle);
void* blockAlloc__Fi(int size);
void lost_soul_ctor(void* self, Point3 position, Point3 velocity, int angle, void* vag, int a, int b) asm("__8LostSoulG6Point3T1iP10_vagHeaderii");
void light_effect_ctor(void* self, Point3 point, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12,
                       void* object, int mode) asm("__11LightEffectG6Point3ffffffffffffP10GameObjecti");
void gameRadialDamage__FP10GameObjectG6Point3iffiii(void* creature, Point3 center, int a, float b, float c, int d, int e, int f);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderG6Point3f(void* sound, Point3 position, float volume);
int cast_repulse_undead(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__18SkillRepulseUndeadiP8CreatureG6Point3s");
}
static char lump_name[] = "fx.lmp";
static char sound_name[] = "spel_he_repulse.anm";
extern "C" int cast_repulse_undead(void* skill, int rank, void* creature, Point3 point, short) {
    unsigned char* s = static_cast<unsigned char*>(skill);
    int angle = 0;
    do {
        Point3 velocity;
        velocity.x = icos__Fi(angle) * 450.0f;
        velocity.y = isin__Fi(angle) * 450.0f;
        velocity.z = 50.0f;
        Point3 at;
        at.x = point.x + 0.0f;
        at.y = point.y;
        at.z = point.z + 36.0f;
        void* soul = blockAlloc__Fi(176);
        if (soul) lost_soul_ctor(soul, at, velocity, angle, nullptr, 1, 0);
        angle += 0x10000 / (rank + 4);
    } while (!(0xffff < angle));
    void* light = blockAlloc__Fi(188);
    if (light) {
        Point3 at;
        at.x = point.x + 0.0f;
        at.y = point.y;
        at.z = point.z + 36.0f;
        light_effect_ctor(light, at, 100.0f, 100.0f, 255.0f, -96.0f, 0.0f, 0.0f, 0.0f, 0.0f, 120.0f, 360.0f, 0.0f, 1.0f, nullptr, 0);
    }
    const int a = w13scb::call_skill_entry(s, 88, 0, rank);
    const int b = w13scb::call_skill_entry(s, 88, 1, rank);
    gameRadialDamage__FP10GameObjectG6Point3iffiii(creature, point, a, static_cast<float>(b * 12), static_cast<float>(b * 24), 0x0a100900, 64, 0);
    soundPlaySequence__FP15AnimationHeaderG6Point3f(lumpFindResource__FPcT0(lump_name, sound_name), point, 1.0f);
    return 0;
}
