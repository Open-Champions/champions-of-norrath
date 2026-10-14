// Observed canCast__14SkillFireStormP8Creature at 0x0029cc70, 260 bytes in .text; the skill argument is used only for
// its table. If bit 0x40 of the word at creature+12 is clear the function returns one. Otherwise, in order:
// (1) playerGetSkillRank__Fii is called with (the word at creature+960, 7); (2) the skill's table entry at byte 88 (a
// signed halfword receiver adjustment, a function word at +4) is called with (skill + adjustment, 1, that rank) and
// its integer result is converted to float r; (3) with a = the signed halfword at creature+56, a point q is built:
// q.x = icos(a) * r * 12.0, q.y = isin(a) * r * 12.0, the floats at creature+16 and creature+20 are added to them and
// q.z = 0; (4) worldFindHeight__FP12_worldHeaderG6Point3f is called with (the word `world`, q, r); (5) the height minus
// the float at creature+24 is truncated to an integer d. The function returns one when |d| < 73 and zero otherwise.
// The table entry at byte 88 is slot 11 of the class table, which names evaluateRampAtRank__5Skillii; that reading is
// inferred from the table. The same ring-point placement is used by cast__14SkillFireStorm.
// Inferred: a player-controlled creature (flag bit 0x40) may cast the storm only when the ground at the target ring
// point is within 72 units of the creature's height at +24. Unknown: what the fields at +16/+20/+24/+56 are called
// (they read as position and heading), what ramp index 1 and skill index 7 select, and the unit of the angle.
// Nothing here ran on the EE.
#include "../w13-skill-cast-a-common/skill_access.hpp"

extern "C" {
int playerGetSkillRank__Fii(int player, int skill);
float icos__Fi(int angle);
float isin__Fi(int angle);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, Point3 point, float radius);
extern void* world;
int can_cast_fire_storm(void* skill, void* creature) asm("canCast__14SkillFireStormP8Creature");
}
typedef int AliasedInt __attribute__((may_alias));
typedef short AliasedShort __attribute__((may_alias));
typedef float AliasedFloat __attribute__((may_alias));
extern "C" int can_cast_fire_storm(void* skill, void* creature) {
    unsigned char* bytes = static_cast<unsigned char*>(creature);
    if (!(*reinterpret_cast<AliasedInt*>(bytes + 12) & 0x40)) return 1;
    const int rank = playerGetSkillRank__Fii(*reinterpret_cast<AliasedInt*>(bytes + 960), 7);
    const float r = static_cast<float>(w13sca::call_skill_entry(static_cast<unsigned char*>(skill), 88, 1, rank));
    Point3 q;
    q.x = icos__Fi(*reinterpret_cast<AliasedShort*>(bytes + 56)) * r * 12.0f;
    q.y = isin__Fi(*reinterpret_cast<AliasedShort*>(bytes + 56)) * r * 12.0f;
    q.x += *reinterpret_cast<AliasedFloat*>(bytes + 16);
    q.y += *reinterpret_cast<AliasedFloat*>(bytes + 20);
    q.z = 0.0f;
    const float h = worldFindHeight__FP12_worldHeaderG6Point3f(world, q, r);
    int d = static_cast<int>(h - *reinterpret_cast<AliasedFloat*>(bytes + 24));
    if (d < 0) d = -d;
    return d < 73;
}
