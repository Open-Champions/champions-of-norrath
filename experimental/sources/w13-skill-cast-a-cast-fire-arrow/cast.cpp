// Observed cast__14SkillFireArrowiP8CreatureG6Point3s at 0x0029e368, 204 bytes in .text. The fifth argument is a signed halfword
// (the trailing "s" of the mangled name).
// Steps: the by-value Point3 is copied to the frame and 12.0 is subtracted from the copy's third component;
// 176 bytes are allocated with blockAlloc__Fi; when the block is not null, the skill's own table (the word at
// skill+96) is used: the entry at table byte 120 holds a signed halfword adjusting the receiver and a function word at
// +4, and that function is called with (skill + adjustment, rank, 20) giving an integer R; then __19FireArrowProjectileG6Point3sP10GameObjectiP5Skill
// is called with (block, the adjusted Point3 copy by address, the halfword widened to int, creature, R, skill).
// The function always returns zero, not the block.
// The table entry at byte 120 is slot 15 of the class table, which names tweekRank__5Skillii; reading R as the rank
// as the class adjusts it is inferred from that table, not from this function.
// Inferred: the projectile starts 12 units below the given point. Unknown: what the constant 20 selects, what the
// halfword means, the layout of the skill and creature, and who owns the projectile. Nothing here ran on the EE.
#include "../w13-skill-cast-a-common/skill_access.hpp"

extern "C" {
void* blockAlloc__Fi(int size);
void projectile_ctor_fire_arrow(void* self, Point3 point, short angle, void* creature, int rank, void* skill) asm("__19FireArrowProjectileG6Point3sP10GameObjectiP5Skill");
int cast_fire_arrow(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__14SkillFireArrowiP8CreatureG6Point3s");
}
extern "C" int cast_fire_arrow(void* skill, int rank, void* creature, Point3 point, short angle) {
    point.z -= 12.0f;
    void* projectile = blockAlloc__Fi(176);
    if (projectile) {
        const int adjusted_rank = w13sca::call_skill_entry(static_cast<unsigned char*>(skill), 120, rank, 20);
        projectile_ctor_fire_arrow(projectile, point, angle, creature, adjusted_rank, skill);
    }
    return 0;
}
