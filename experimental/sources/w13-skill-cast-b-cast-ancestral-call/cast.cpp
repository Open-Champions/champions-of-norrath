// Observed cast__18SkillAncestralCalliP8CreatureG6Point3s at 0x002a8490, 160 bytes in .text.
// Steps, in order: (1) the by-value Point3 is copied to the frame and not read; (2) the skill's own table (the word at
// skill+96) is used: the entry at table byte 120 holds a signed halfword adjusting the receiver and a function word
// at +4; that function is called with (skill + adjustment, rank, 20) and its integer result R is kept; (3) 132 bytes
// are allocated with blockAlloc__Fi; (4) when the block is not null, __18SpellAncestralCalliP8CreatureP5Skill is called with
// (block, R, creature, skill). The block pointer is returned, null when the allocation failed.
// Observed in the private image: slot 15 (table byte 120) of this class's table holds tweekRank__5Skillii, whose
// base body returns its second integer argument; so R is the rank as the class may adjust it (inferred from that
// table, not read in this function). Unknown: what the constant 20 selects, and the layout of the skill and creature.
// Nothing here ran on the EE.
#include "../w13-skill-cast-b-common/skill_access.hpp"

extern "C" {
void* blockAlloc__Fi(int size);
void spell_ctor_ancestral_call(void* self, int rank, void* creature, void* skill) asm("__18SpellAncestralCalliP8CreatureP5Skill");
void* cast_ancestral_call(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__18SkillAncestralCalliP8CreatureG6Point3s");
}
extern "C" void* cast_ancestral_call(void* skill, int rank, void* creature, Point3, short) {
    const int adjusted_rank = w13scb::call_skill_entry(static_cast<unsigned char*>(skill), 120, rank, 20);
    void* spell = blockAlloc__Fi(132);
    if (spell) spell_ctor_ancestral_call(spell, adjusted_rank, creature, skill);
    return spell;
}
