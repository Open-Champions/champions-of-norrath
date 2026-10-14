// Observed cast__12SkillBlessediP8CreatureG6Point3s at 0x002a6e70, 204 bytes in .text.
// Steps: the by-value Point3 is copied to the frame and not read; __tf12SpellBlessed (the type-info function of the
// matching spell class) is called and checkIfSpellIsRunning__8CreatureRC9type_info is asked about that type on the
// creature. When the lookup finds a running spell the function returns zero and changes nothing. Otherwise eight
// halfwords in the creature are written (listed below), then 252 bytes
// are allocated with blockAlloc__Fi and, when the block is not null, __12SpellBlessediP8CreatureP5Skill is called
// with (block, rank, creature, skill). The block pointer is returned, null when the allocation failed (the halfword
// stores have happened by then).
// Halfword stores: 75 at creature+140, +142, +148 and +150; 200 at +144 and +152; 15 at +146 and +154.
// Inferred: the 75/200/15 pairs look like values written into several slots of one record before the spell starts.
// Unknown: what the creature halfwords are (they are not named anywhere in this package), what 75, 200 and 15 mean,
// and the layouts of the skill and the spell. Nothing here ran on the EE.
struct Point3 { float x, y, z; };
extern "C" {
void* blockAlloc__Fi(int size);
const void* spell_type_info() asm("__tf12SpellBlessed");
void* checkIfSpellIsRunning__8CreatureRC9type_info(void* creature, const void* typeinfo);
void spell_ctor_blessed(void* self, int rank, void* creature, void* skill) asm("__12SpellBlessediP8CreatureP5Skill");
void* cast_blessed(void* skill, int rank, void* creature, Point3 point) asm("cast__12SkillBlessediP8CreatureG6Point3s");
}
typedef short AliasedShort __attribute__((may_alias));
extern "C" void* cast_blessed(void* skill, int rank, void* creature, Point3) {
    if (checkIfSpellIsRunning__8CreatureRC9type_info(creature, spell_type_info()) != nullptr) return nullptr;
    unsigned char* bytes = static_cast<unsigned char*>(creature);
    *reinterpret_cast<AliasedShort*>(bytes + 152) = 200;
    *reinterpret_cast<AliasedShort*>(bytes + 154) = 15;
    *reinterpret_cast<AliasedShort*>(bytes + 144) = 200;
    *reinterpret_cast<AliasedShort*>(bytes + 146) = 15;
    *reinterpret_cast<AliasedShort*>(bytes + 150) = 75;
    *reinterpret_cast<AliasedShort*>(bytes + 140) = 75;
    *reinterpret_cast<AliasedShort*>(bytes + 148) = 75;
    *reinterpret_cast<AliasedShort*>(bytes + 142) = 75;
    void* spell = blockAlloc__Fi(252);
    if (spell) spell_ctor_blessed(spell, rank, creature, skill);
    return spell;
}
