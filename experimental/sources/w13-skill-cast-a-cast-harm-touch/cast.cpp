// Observed cast__14SkillHarmTouchiP8CreatureG6Point3s at 0x0029af78, 132 bytes in .text.
// Copies the by-value Point3 to its frame without reading it, allocates 28 bytes with blockAlloc__Fi and, when the
// allocation succeeded, constructs the spell in place with __14SpellHarmTouchiP8CreatureP5Skill (block, rank, creature, skill).
// It returns the block pointer, null when the allocation failed.
// Inferred: this starts the spell of the matching class for the creature at the given rank. Unknown: the layout of
// the skill and creature objects (neither is read here) and the spell's own state.
struct Point3 { float x, y, z; };
extern "C" {
void* blockAlloc__Fi(int size);
void spell_ctor_harm_touch(void* self, int rank, void* creature, void* skill) asm("__14SpellHarmTouchiP8CreatureP5Skill");
void* cast_harm_touch(void* skill, int rank, void* creature, Point3 point) asm("cast__14SkillHarmTouchiP8CreatureG6Point3s");
}
extern "C" void* cast_harm_touch(void* skill, int rank, void* creature, Point3) {
    void* spell = blockAlloc__Fi(28);
    if (spell) spell_ctor_harm_touch(spell, rank, creature, skill);
    return spell;
}
