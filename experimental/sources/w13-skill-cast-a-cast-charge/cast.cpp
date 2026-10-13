// Observed cast__11SkillChargeiP8CreatureG6Point3s at 0x00292038, 160 bytes in .text.
// Steps: the by-value Point3 is copied to the frame and not read; 136 bytes are allocated with blockAlloc__Fi; when
// the block is not null, __11SpellChargeiP8CreatureP5Skill is called with (block, rank, creature, skill). Then, still
// only when the block is not null, if bit 0x40 of the word at creature+12 is set, the word at creature+1400 is set to 9.
// The block pointer is returned, null when the allocation failed.
// Inferred: starts the charge spell and, for a creature with that flag bit, switches it into a state numbered 9.
// Unknown: what bit 0x40 of creature+12 means (the same bit gates the cyclone check on the player record), what
// state 9 is, and the layouts of the skill and the spell. Nothing here ran on the EE.
struct Point3 { float x, y, z; };
typedef int AliasedInt __attribute__((may_alias));
extern "C" {
void* blockAlloc__Fi(int size);
void spell_ctor_charge(void* self, int rank, void* creature, void* skill) asm("__11SpellChargeiP8CreatureP5Skill");
void* cast_charge(void* skill, int rank, void* creature, Point3 point) asm("cast__11SkillChargeiP8CreatureG6Point3s");
}
extern "C" void* cast_charge(void* skill, int rank, void* creature, Point3) {
    void* spell = blockAlloc__Fi(136);
    if (spell) {
        spell_ctor_charge(spell, rank, creature, skill);
        unsigned char* bytes = static_cast<unsigned char*>(creature);
        if (*reinterpret_cast<AliasedInt*>(bytes + 12) & 0x40) *reinterpret_cast<AliasedInt*>(bytes + 1400) = 9;
    }
    return spell;
}
