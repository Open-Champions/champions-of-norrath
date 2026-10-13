// Observed cast__12SkillHealingiP8CreatureG6Point3s at 0x002af290, 160 bytes in .text.
// Steps, in order: (1) the by-value Point3 is copied to the frame and not read; (2) the skill's own table (the word at
// skill+96) is used: the entry at table byte 120 holds a signed halfword adjusting the receiver and a function word
// at +4; that function is called with (skill + adjustment, rank, 14) and its integer result R is kept; (3) 40 bytes
// are allocated with blockAlloc__Fi; (4) when the block is not null, __12SpellHealingiP8CreatureP5Skill is called with
// (block, R, creature, skill). The block pointer is returned, null when the allocation failed.
// The table entry at byte 120 is slot 15 of the class table, which names tweekRank__5Skillii (its base body
// returns its second integer argument), so R is the rank as the class adjusts it. That reading is inferred from the
// table, not from this function. Unknown: what the constant 14 selects, and the layout of the skill and creature.
// Nothing here ran on the EE.
#include "../w13-skill-cast-a-common/skill_access.hpp"

extern "C" {
void* blockAlloc__Fi(int size);
void spell_ctor_healing(void* self, int rank, void* creature, void* skill) asm("__12SpellHealingiP8CreatureP5Skill");
void* cast_healing(void* skill, int rank, void* creature, Point3 point) asm("cast__12SkillHealingiP8CreatureG6Point3s");
}
extern "C" void* cast_healing(void* skill, int rank, void* creature, Point3) {
    const int adjusted_rank = w13sca::call_skill_entry(static_cast<unsigned char*>(skill), 120, rank, 14);
    void* spell = blockAlloc__Fi(40);
    if (spell) spell_ctor_healing(spell, adjusted_rank, creature, skill);
    return spell;
}
