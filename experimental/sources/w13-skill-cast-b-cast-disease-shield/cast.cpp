// Observed cast__18SkillDiseaseShieldiP8CreatureG6Point3s at 0x00296808, 148 bytes in .text.
// Steps, in order: (1) the by-value Point3 is copied to the frame and not read; (2) four halfwords of the creature are
// written: -100 at byte offsets 148, 150 and 152 and zero at 154; (3) 432 bytes are allocated with blockAlloc__Fi;
// (4) when the block is not null, __18SpellDiseaseShieldiP8CreatureP5Skill is called with (block, rank, creature, skill).
// The block pointer is returned, null when the allocation failed. The halfwords are written whether or not the
// allocation succeeded.
// Inferred: the four halfwords are a colour group of the creature (the same offsets are set by the disease trail cast);
// the three -100 values and the zero are not named anywhere. Unknown: the meaning of that group and the creature layout.
struct Point3 { float x, y, z; };
extern "C" {
void* blockAlloc__Fi(int size);
void spell_ctor_disease_shield(void* self, int rank, void* creature, void* skill) asm("__18SpellDiseaseShieldiP8CreatureP5Skill");
void* cast_disease_shield(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__18SkillDiseaseShieldiP8CreatureG6Point3s");
}
extern "C" void* cast_disease_shield(void* skill, int rank, void* creature, Point3, short) {
    short* group = reinterpret_cast<short*>(static_cast<unsigned char*>(creature) + 148);
    group[0] = -100;
    group[1] = -100;
    group[2] = -100;
    group[3] = 0;
    void* spell = blockAlloc__Fi(432);
    if (spell) spell_ctor_disease_shield(spell, rank, creature, skill);
    return spell;
}
