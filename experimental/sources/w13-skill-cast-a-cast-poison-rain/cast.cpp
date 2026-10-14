// Observed cast__15SkillPoisonRainiP8CreatureG6Point3s at 0x002bed80, 324 bytes in .text. The fifth argument is a
// signed halfword (the trailing "s" of the mangled name), used as an integer angle.
// Steps, in order: (1) the by-value Point3 is copied to the frame; (2) the skill's table entry at byte 104 (a signed
// halfword receiver adjustment, a function word at +4) is called with (skill + adjustment, 1, rank, creature) and its
// integer result R is converted to float r; (3) a local point q is built: q.x = icos(angle) * r * 12.0, q.y =
// isin(angle) * r * 12.0, then the incoming point's x and y are added, q.z = 0; (4) worldFindHeight__FP12_worldHeaderG6Point3f
// is called with (the word `world`, q, r) giving h; q.z becomes (0 + h) + 12.0; (5) 108 bytes are allocated with
// blockAlloc__Fi and, when the block is not null, __10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader is
// called with (block, rank, creature, q by address, skill, null). The function always returns zero, not the block.
// The table entry at byte 104 is slot 13 of the class table, which names evaluateRampAtRank__5SkilliiP10GameObject;
// that reading is inferred from the table, not from this function. The incoming point's third component is not read.
// The sequence matches the fire storm cast except for the table entry, the 12.0 lift, the block size and the extra
// null argument. Unknown: what ramp index 1 selects, the unit of the angle, the layouts of skill and creature, and who
// owns the rain. Nothing here ran on the EE.
#include "../w13-skill-cast-a-common/skill_access.hpp"

extern "C" {
void* blockAlloc__Fi(int size);
float icos__Fi(int angle);
float isin__Fi(int angle);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, Point3 point, float radius);
extern void* world;
void rain_ctor_poison_rain(void* self, int rank, void* creature, Point3 point, void* skill, void* animation) asm("__10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader");
int cast_poison_rain(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__15SkillPoisonRainiP8CreatureG6Point3s");
}
extern "C" int cast_poison_rain(void* skill, int rank, void* creature, Point3 point, short angle) {
    const float r = static_cast<float>(w13sca::call_skill_entry3(static_cast<unsigned char*>(skill), 104, 1, rank, creature));
    Point3 q;
    q.x = icos__Fi(angle) * r * 12.0f;
    q.y = isin__Fi(angle) * r * 12.0f;
    q.x += point.x;
    q.z = 0.0f;
    q.y += point.y;
    const float h = worldFindHeight__FP12_worldHeaderG6Point3f(world, q, r);
    q.z = q.z + h + 12.0f;
    void* rain = blockAlloc__Fi(108);
    if (rain) rain_ctor_poison_rain(rain, rank, creature, q, skill, nullptr);
    return 0;
}
