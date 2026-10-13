// Observed cast__17SkillExplodeArrowiP8CreatureG6Point3s at 0x002a2720, 192 bytes in .text.
// Steps, in order: (1) the by-value Point3 is copied to the frame; (2) the skill's own table (the word at skill+96) is used: the entry at table byte 120
// holds a signed halfword adjusting the receiver and a function word at +4; that function is called with (skill + adjustment, rank, 9)
// and its integer result R is kept; (3) 12.0 is subtracted from the third float of the frame copy; (4) 116 bytes are allocated with
// blockAlloc__Fi; (5) when the block is not null, the projectile constructor is called with (block, the frame copy of the point
// passed by reference, the trailing short sign-extended, creature, R, skill).
// The block pointer is not returned: the function returns zero in every case.
// Observed in the private image: slot 15 (table byte 120) of this class's table holds tweekRank__5Skillii, whose
// base body returns its second integer argument; so R is the rank as the class may adjust it (inferred from that
// table, not read in this function). Inferred: 12.0 is subtracted from the point's third float before it is used (the
// third component is the vertical one if Point3 is x, y, z, but that is not established here). Unknown: what the
// constant 9 selects, the meaning of the short (the arguments suggest an angle), and the layouts of skill and creature.
// Nothing here ran on the EE.
#include "../w13-skill-cast-b-common/skill_access.hpp"

extern "C" {
void* blockAlloc__Fi(int size);
void projectile_ctor(void* self, Point3 point, short angle, void* creature, int rank, void* skill) asm("__22ExplodeArrowProjectileG6Point3sP10GameObjectiP5Skill");
int cast_arrow(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__17SkillExplodeArrowiP8CreatureG6Point3s");
}
extern "C" int cast_arrow(void* skill, int rank, void* creature, Point3 point, short angle) {
    const int adjusted_rank = w13scb::call_skill_entry(static_cast<unsigned char*>(skill), 120, rank, 9);
    point.z -= 12.0f;
    void* projectile = blockAlloc__Fi(116);
    if (projectile) projectile_ctor(projectile, point, angle, creature, adjusted_rank, skill);
    return 0;
}
