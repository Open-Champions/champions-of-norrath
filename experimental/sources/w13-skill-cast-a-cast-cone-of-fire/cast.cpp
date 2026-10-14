// Observed cast__15SkillConeOfFireiP8CreatureG6Point3s at 0x002b6a70, 132 bytes in .text.
// Copies the by-value Point3 to its frame without reading it, allocates 88 bytes with blockAlloc__Fi and, when the
// allocation succeeded, constructs the spell in place with __15SpellConeOfFireiP8CreatureP5Skill (block, rank, creature, skill).
// It returns the block pointer, null when the allocation failed.
// Inferred: this starts the spell of the matching class for the creature at the given rank. Unknown: the layout of
// the skill and creature objects (neither is read here) and the spell's own state.
struct Point3 { float x, y, z; };
extern "C" {
void* blockAlloc__Fi(int size);
void spell_ctor_cone_of_fire(void* self, int rank, void* creature, void* skill) asm("__15SpellConeOfFireiP8CreatureP5Skill");
void* cast_cone_of_fire(void* skill, int rank, void* creature, Point3 point) asm("cast__15SkillConeOfFireiP8CreatureG6Point3s");
}
extern "C" void* cast_cone_of_fire(void* skill, int rank, void* creature, Point3) {
    void* spell = blockAlloc__Fi(88);
    if (spell) spell_ctor_cone_of_fire(spell, rank, creature, skill);
    return spell;
}
