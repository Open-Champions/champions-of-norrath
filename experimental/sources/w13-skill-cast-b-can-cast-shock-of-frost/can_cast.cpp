// Observed canCast__17SkillShockOfFrostP8Creature at 0x002be190, 8 bytes in .text.
// Returns one without reading either argument.
// Inferred: this skill has no precondition on the creature. Unknown: whether the caller relies on a stricter check elsewhere.
extern "C" int can_cast_shock_of_frost(void* skill, void* creature) asm("canCast__17SkillShockOfFrostP8Creature");
extern "C" int can_cast_shock_of_frost(void*, void*) { return 1; }
