// Observed cast__17SkillShockOfFrostiP8CreatureG6Point3s at 0x002be0c8, 120 bytes in .text.
// Allocates 208 bytes with blockAlloc__Fi and, when the block is not null, calls
// __12ShockOfFrostG6Point3siP10GameObjectP5Skill with (block, the frame copy of the point passed by reference, the
// trailing short argument sign-extended, rank, creature, skill). It returns zero in every case, including success;
// the block pointer is not returned.
// Inferred: the object created is an effect that registers itself elsewhere, which would explain the unused block
// pointer. Unknown: that registration, the meaning of the short (the arguments suggest an angle), and the return type.
struct Point3 { float x, y, z; };
extern "C" {
void* blockAlloc__Fi(int size);
void shock_of_frost_ctor(void* self, Point3 point, short angle, int rank, void* creature, void* skill) asm("__12ShockOfFrostG6Point3siP10GameObjectP5Skill");
int cast_shock_of_frost(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__17SkillShockOfFrostiP8CreatureG6Point3s");
}
extern "C" int cast_shock_of_frost(void* skill, int rank, void* creature, Point3 point, short angle) {
    void* effect = blockAlloc__Fi(208);
    if (effect) shock_of_frost_ctor(effect, point, angle, rank, creature, skill);
    return 0;
}
