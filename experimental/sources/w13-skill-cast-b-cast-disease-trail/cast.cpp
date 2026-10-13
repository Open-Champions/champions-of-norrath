// Observed cast__17SkillDiseaseTrailiP8CreatureG6Point3s at 0x00295490, 148 bytes in .text.
// Steps, in order: (1) the by-value Point3 is copied to the frame; (2) four halfwords of the creature are written:
// -100 at byte offsets 148, 150 and 152 and zero at 154 (the same group the disease shield cast sets); (3) 48 bytes
// are allocated with blockAlloc__Fi; (4) when the block is not null, __17SpellDiseaseTrailG6Point3iP8CreatureP5Skill is
// called with (block, the frame copy of the point passed by reference, rank, creature, skill). The block pointer is
// returned, null when the allocation failed. The halfwords are written whether or not the allocation succeeded.
// Inferred: the point is the place the trail starts; the halfword group is a colour group of the creature.
// Unknown: the meaning of that group and the creature layout. The trailing short argument is not read.
struct Point3 { float x, y, z; };
extern "C" {
void* blockAlloc__Fi(int size);
void spell_ctor_disease_trail(void* self, Point3 point, int rank, void* creature, void* skill) asm("__17SpellDiseaseTrailG6Point3iP8CreatureP5Skill");
void* cast_disease_trail(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__17SkillDiseaseTrailiP8CreatureG6Point3s");
}
extern "C" void* cast_disease_trail(void* skill, int rank, void* creature, Point3 point, short) {
    short* group = reinterpret_cast<short*>(static_cast<unsigned char*>(creature) + 148);
    group[0] = -100;
    group[1] = -100;
    group[2] = -100;
    group[3] = 0;
    void* spell = blockAlloc__Fi(48);
    if (spell) spell_ctor_disease_trail(spell, point, rank, creature, skill);
    return spell;
}
