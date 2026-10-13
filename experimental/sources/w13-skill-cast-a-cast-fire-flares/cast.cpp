// Observed cast__15SkillFireFlaresiP8CreatureG6Point3s at 0x002a48e0, 136 bytes in .text.
// The by-value Point3 is copied to the frame; 120 bytes are allocated with blockAlloc__Fi; when the block is not
// null, __15SpellFireFlaresiP8CreatureG6Point3P5Skill is called with (block, rank, creature, the Point3 copy by
// address, skill). The block pointer is returned, null when the allocation failed. Unlike the other casts of this
// package the point is passed on to the spell. Inferred: the spell is started at the given point. Unknown: the layouts
// of skill, creature and spell. Nothing here ran on the EE.
struct Point3 { float x, y, z; };
extern "C" {
void* blockAlloc__Fi(int size);
void spell_ctor_fire_flares(void* self, int rank, void* creature, Point3 point, void* skill) asm("__15SpellFireFlaresiP8CreatureG6Point3P5Skill");
void* cast_fire_flares(void* skill, int rank, void* creature, Point3 point) asm("cast__15SkillFireFlaresiP8CreatureG6Point3s");
}
extern "C" void* cast_fire_flares(void* skill, int rank, void* creature, Point3 point) {
    void* spell = blockAlloc__Fi(120);
    if (spell) spell_ctor_fire_flares(spell, rank, creature, point, skill);
    return spell;
}
