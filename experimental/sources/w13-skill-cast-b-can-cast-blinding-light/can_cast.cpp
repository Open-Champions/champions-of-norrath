// Observed canCast__18SkillBlindingLightP8Creature at 0x002ba1b0, 8 bytes in .text.
// Returns one without reading either argument.
// Inferred: this skill has no precondition on the creature. Unknown: whether the caller relies on a stricter check elsewhere.
extern "C" int can_cast_blinding_light(void* skill, void* creature) asm("canCast__18SkillBlindingLightP8Creature");
extern "C" int can_cast_blinding_light(void*, void*) { return 1; }
