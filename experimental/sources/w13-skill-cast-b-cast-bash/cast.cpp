// Observed cast__9SkillBashiP8CreatureG6Point3s at 0x002a5e40, 200 bytes in .text.
// Steps, in order: (1) the by-value Point3 is copied to the frame and not read; (2) the skill's own table (the word at
// skill+96) is used: the entry at table byte 120 holds a signed halfword adjusting the receiver and a function word
// at +4; that function is called with (skill + adjustment, rank, 19) and its integer result R is kept; (3) the
// resource "spel_bw_bash.anm" is looked up in the lump "fx.lmp" with lumpFindResource__FPcT0 and played with
// soundPlaySequence__FP15AnimationHeaderP10GameObjectf(result, creature, 1.0); (4) 40 bytes are allocated with
// blockAlloc__Fi; (5) when the block is not null, __9SpellBashiP8CreatureP5Skill is called with (block, R, creature,
// skill). The block pointer is returned, null when the allocation failed. The sound is played in every case.
// Observed in the private image: slot 15 (table byte 120) of this class's table holds tweekRank__5Skillii, whose
// base body returns its second integer argument; so R is the rank as the class may adjust it (inferred from that
// table, not read in this function). Unknown: what the constant 19 selects, whether the lookup result may be null, and
// the layouts of skill and creature. Nothing here ran on the EE.
#include "../w13-skill-cast-b-common/skill_access.hpp"

extern "C" {
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sound, void* object, float volume);
void* blockAlloc__Fi(int size);
void spell_ctor_bash(void* self, int rank, void* creature, void* skill) asm("__9SpellBashiP8CreatureP5Skill");
void* cast_bash(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__9SkillBashiP8CreatureG6Point3s");
}
static char lump_name[] = "fx.lmp";
static char sound_name[] = "spel_bw_bash.anm";
extern "C" void* cast_bash(void* skill, int rank, void* creature, Point3, short) {
    const int adjusted_rank = w13scb::call_skill_entry(static_cast<unsigned char*>(skill), 120, rank, 19);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(lump_name, sound_name), creature, 1.0f);
    void* spell = blockAlloc__Fi(40);
    if (spell) spell_ctor_bash(spell, adjusted_rank, creature, skill);
    return spell;
}
