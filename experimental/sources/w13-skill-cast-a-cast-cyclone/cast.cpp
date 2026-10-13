// Observed cast__12SkillCycloneiP8CreatureG6Point3s at 0x0029a820, 132 bytes in .text.
// Copies the by-value Point3 to its frame without reading it, allocates 124 bytes with blockAlloc__Fi and, when the
// allocation succeeded, constructs the spell in place with __12SpellCycloneiP8CreatureP5Skill (block, rank, creature, skill).
// It returns the block pointer, null when the allocation failed.
// Inferred: this starts the spell of the matching class for the creature at the given rank. Unknown: the layout of
// the skill and creature objects (neither is read here) and the spell's own state.
struct Point3 { float x, y, z; };
extern "C" {
void* blockAlloc__Fi(int size);
void spell_ctor_cyclone(void* self, int rank, void* creature, void* skill) asm("__12SpellCycloneiP8CreatureP5Skill");
void* cast_cyclone(void* skill, int rank, void* creature, Point3 point) asm("cast__12SkillCycloneiP8CreatureG6Point3s");
}
extern "C" void* cast_cyclone(void* skill, int rank, void* creature, Point3) {
    void* spell = blockAlloc__Fi(124);
    if (spell) spell_ctor_cyclone(spell, rank, creature, skill);
    return spell;
}
