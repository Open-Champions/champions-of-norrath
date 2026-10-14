// Observed cast__18SkillDismissUndeadiP8CreatureG6Point3s at 0x002bbc90, 144 bytes in .text.
// Allocates 192 bytes with blockAlloc__Fi and, when the block is not null, calls
// __8HolyBoltG6Point3iiP8CreatureP5Skill with (block, the frame copy of the point passed by reference, the
// trailing short argument sign-extended, rank, creature, skill). It returns zero in every case, including success;
// the block pointer is not returned.
// Inferred: the object created registers itself elsewhere, which would explain the unused block pointer.
// Unknown: that registration, the meaning of the short (the arguments suggest an angle), and the return type.
struct Point3 { float x, y, z; };
extern "C" {
void* blockAlloc__Fi(int size);
void ctor_dismiss_undead(void* self, Point3 point, int angle, int rank, void* creature, void* skill) asm("__8HolyBoltG6Point3iiP8CreatureP5Skill");
int cast_dismiss_undead(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__18SkillDismissUndeadiP8CreatureG6Point3s");
}
extern "C" int cast_dismiss_undead(void* skill, int rank, void* creature, Point3 point, short angle) {
    void* object = blockAlloc__Fi(192);
    if (object) ctor_dismiss_undead(object, point, angle, rank, creature, skill);
    return 0;
}
