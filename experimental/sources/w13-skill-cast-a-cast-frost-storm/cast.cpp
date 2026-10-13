// Observed cast__15SkillFrostStormiP8CreatureG6Point3s at 0x002b3298, 580 bytes in .text. The fifth argument is a
// signed halfword (the trailing "s" of the mangled name), used as an integer angle.
// Steps, in order: (1) the by-value Point3 is copied to the frame; (2) the skill's table entry at byte 120 (a signed
// halfword receiver adjustment, a function word at +4) is called with (skill + adjustment, rank, 20), giving T; (3)
// icos(angle) and isin(angle) give a displacement d = (icos * 3.0 * 12.0, isin * 3.0 * 12.0, 0) and q = (point.x + d.x,
// point.y + d.y, point.z); (4) the table entry at byte 88 is called with (skill + adjustment, i, T) for i = 0, 1, 2, 3
// giving A, B, C and D; (5) with r = (float)D, e = (icos(angle) * r, isin(angle) * r, 0); (6) 172 bytes are allocated
// with blockAlloc__Fi and, when the block is not null, __15FrostStormSpellG6Point3ifiP10GameObjectT1 is called with
// (block, q, 60 * C, (float)(12 * B), A, creature, e); (7) whether or not the allocation succeeded,
// lumpFindResource__FPcT0 is called with the strings "fx.lmp" and "spel_ew_froststorm.anm" and
// soundPlaySequence__FP15AnimationHeaderP10GameObjectf is called with (that result, creature, 1.0). The function always
// returns zero. The skill itself and the rank are not passed to the spell.
// The table entries at bytes 120 and 88 are named, in the class tables, tweekRank__5Skillii and
// evaluateRampAtRank__5Skillii; those readings are inferred from the tables. Inferred: ramps 0..3 of the storm supply
// four per-rank numbers (here the integer A and D, and B and C scaled by 12 and 60), and the sound plays at the
// creature even if no spell was allocated. Unknown: what the four ramps mean, what the scale factors convert, the
// angle unit, and the layouts of skill and creature. Nothing here ran on the EE.
#include "../w13-skill-cast-a-common/skill_access.hpp"

extern "C" {
void* blockAlloc__Fi(int size);
float icos__Fi(int angle);
float isin__Fi(int angle);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume);
void spell_ctor_frost_storm(void* self, Point3 q, int a, float b, int c, void* creature, Point3 e) asm("__15FrostStormSpellG6Point3ifiP10GameObjectT1");
int cast_frost_storm(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__15SkillFrostStormiP8CreatureG6Point3s");
}
extern "C" int cast_frost_storm(void* skill, int rank, void* creature, Point3 point, short angle) {
    unsigned char* const skill_bytes = static_cast<unsigned char*>(skill);
    const int tweaked = w13sca::call_skill_entry(skill_bytes, 120, rank, 20);
    Point3 d;
    d.x = icos__Fi(angle) * 3.0f * 12.0f;
    d.y = isin__Fi(angle) * 3.0f * 12.0f;
    d.z = 0.0f;
    Point3 q;
    q.x = point.x + d.x;
    q.y = point.y + d.y;
    q.z = point.z;
    const int ramp0 = w13sca::call_skill_entry(skill_bytes, 88, 0, tweaked);
    const int ramp1 = w13sca::call_skill_entry(skill_bytes, 88, 1, tweaked);
    const int ramp2 = w13sca::call_skill_entry(skill_bytes, 88, 2, tweaked);
    const float ramp3 = static_cast<float>(w13sca::call_skill_entry(skill_bytes, 88, 3, tweaked));
    Point3 e;
    e.x = icos__Fi(angle) * ramp3;
    e.y = isin__Fi(angle) * ramp3;
    e.z = 0.0f;
    void* storm = blockAlloc__Fi(172);
    if (storm) spell_ctor_frost_storm(storm, q, ramp2 * 60, static_cast<float>(ramp1 * 12), ramp0, creature, e);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_ew_froststorm.anm")), creature, 1.0f);
    return 0;
}
