// Observed cast__14SkillFireStormiP8CreatureG6Point3s at 0x0029c940, 324 bytes in .text. The fifth argument is a
// signed halfword (the trailing "s" of the mangled name), used as an integer angle.
// Steps, in order: (1) the by-value Point3 is copied to the frame; (2) the skill's table entry at byte 88 (a signed
// halfword receiver adjustment, a function word at +4) is called with (skill + adjustment, 1, rank) and its integer
// result R is converted to float r; (3) a local point q is built: q.x = icos(angle) * r * 12.0, q.y = isin(angle) * r *
// 12.0, then the incoming point's x and y are added, q.z = 0; (4) worldFindHeight__FP12_worldHeaderG6Point3f is called
// with (the word `world`, q, r) giving h; q.z becomes (0 + h) + 6.0; (5) 168 bytes are allocated with blockAlloc__Fi
// and, when the block is not null, __9FireStormiP10GameObjectG6Point3P5Skill is called with (block, rank, creature, q by
// address, skill). The function always returns zero, not the block.
// The table entry at byte 88 is slot 11 of the class table, which names evaluateRampAtRank__5Skillii; that reading
// is inferred from the table, not from this function. The incoming point's third component is not read.
// Inferred: the storm is placed where a ring of radius 12 * r (at the angle) around the point meets the ground, 6 units
// above it. Unknown: what ramp index 1 selects, the unit of the angle, the layouts of skill and creature, and who owns
// the storm. Nothing here ran on the EE.
#include "../w13-skill-cast-a-common/skill_access.hpp"

extern "C" {
void* blockAlloc__Fi(int size);
float icos__Fi(int angle);
float isin__Fi(int angle);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, Point3 point, float radius);
extern void* world;
void storm_ctor_fire_storm(void* self, int rank, void* creature, Point3 point, void* skill) asm("__9FireStormiP10GameObjectG6Point3P5Skill");
int cast_fire_storm(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__14SkillFireStormiP8CreatureG6Point3s");
}
extern "C" int cast_fire_storm(void* skill, int rank, void* creature, Point3 point, short angle) {
    const float r = static_cast<float>(w13sca::call_skill_entry(static_cast<unsigned char*>(skill), 88, 1, rank));
    Point3 q;
    q.x = icos__Fi(angle) * r * 12.0f;
    q.y = isin__Fi(angle) * r * 12.0f;
    q.x += point.x;
    q.z = 0.0f;
    q.y += point.y;
    const float h = worldFindHeight__FP12_worldHeaderG6Point3f(world, q, r);
    q.z = q.z + h + 6.0f;
    void* storm = blockAlloc__Fi(168);
    if (storm) storm_ctor_fire_storm(storm, rank, creature, q, skill);
    return 0;
}
