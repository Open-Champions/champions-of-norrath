// Observed cast__22SkillRangerCriticalHitiP8CreatureG6Point3s at 0x002c7c60, 132 bytes in .text.
// Copies the by-value Point3 to its frame without reading it, allocates 92 bytes with blockAlloc__Fi and, when the
// allocation succeeded, constructs the spell in place with __22SpellRangerCriticalHitiP8CreatureP5Skill (block, rank, creature, skill).
// It returns the block pointer, null when the allocation failed.
// Inferred: this starts the spell of the matching class for the creature at the given rank. Unknown: the layout of
// the skill and creature objects (neither is read here) and the spell's own state.
struct Point3 { float x, y, z; };
extern "C" {
void* blockAlloc__Fi(int size);
void spell_ctor_ranger_critical_hit(void* self, int rank, void* creature, void* skill) asm("__22SpellRangerCriticalHitiP8CreatureP5Skill");
void* cast_ranger_critical_hit(void* skill, int rank, void* creature, Point3 point) asm("cast__22SkillRangerCriticalHitiP8CreatureG6Point3s");
}
extern "C" void* cast_ranger_critical_hit(void* skill, int rank, void* creature, Point3) {
    void* spell = blockAlloc__Fi(92);
    if (spell) spell_ctor_ranger_critical_hit(spell, rank, creature, skill);
    return spell;
}
