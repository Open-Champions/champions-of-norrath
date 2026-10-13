// Observed cast__10SkillShockiP8CreatureG6Point3s at 0x0028d8b0, 144 bytes in .text. The fifth argument is a
// signed halfword (the trailing "s" of the mangled name).
// Steps: the by-value Point3 is copied to the frame; 408 bytes are allocated with blockAlloc__Fi; when the block is
// not null, __15ShockProjectileG6Point3sP10GameObjectP5Skilli is called with (block, the Point3 copy by address,
// the halfword widened to int, creature, skill, rank). The function always returns zero, not the block.
// Inferred: this launches a shock projectile at the given point and keeps no handle to it. Unknown: the meaning of
// the halfword, the layouts of skill and creature (neither is read here) and who owns the projectile afterwards.
// Nothing here ran on the EE.
struct Point3 { float x, y, z; };
extern "C" {
void* blockAlloc__Fi(int size);
void projectile_ctor_shock(void* self, Point3 point, short angle, void* creature, void* skill, int rank) asm("__15ShockProjectileG6Point3sP10GameObjectP5Skilli");
int cast_shocki(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__10SkillShockiP8CreatureG6Point3s");
}
extern "C" int cast_shocki(void* skill, int rank, void* creature, Point3 point, short angle) {
    void* projectile = blockAlloc__Fi(408);
    if (projectile) projectile_ctor_shock(projectile, point, angle, creature, skill, rank);
    return 0;
}
