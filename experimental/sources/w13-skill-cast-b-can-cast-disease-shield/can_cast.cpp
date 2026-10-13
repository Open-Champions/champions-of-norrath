// Observed canCast__18SkillDiseaseShieldP8Creature at 0x002968d8, 52 bytes in .text.
// Calls __tf18SpellDiseaseShield (the type-info function of the matching spell class), then
// checkIfSpellIsRunning__8CreatureRC9type_info on the creature with that result. The function returns one when that
// lookup yields null and zero otherwise; the skill argument is not read.
// Inferred: the skill can be cast only while no spell of the matching class is already running on the creature
// (the lookup returns the running spell, as in terminate_spell). Unknown: what the lookup compares beyond the type.
extern "C" {
const void* spell_type_info() asm("__tf18SpellDiseaseShield");
void* checkIfSpellIsRunning__8CreatureRC9type_info(void* creature, const void* typeinfo);
int can_cast_disease_shield(void* skill, void* creature) asm("canCast__18SkillDiseaseShieldP8Creature");
}
extern "C" int can_cast_disease_shield(void*, void* creature) {
    return checkIfSpellIsRunning__8CreatureRC9type_info(creature, spell_type_info()) == nullptr;
}
