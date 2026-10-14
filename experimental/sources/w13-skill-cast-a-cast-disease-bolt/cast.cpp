// Observed cast__16SkillDiseaseBoltiP8CreatureG6Point3s at 0x002937c0, 164 bytes in .text. The fifth argument is a signed halfword
// (the trailing "s" of the mangled name).
// Steps: the by-value Point3 is copied to the frame; halfwords are written into the creature (listed below);
// 124 bytes are allocated with blockAlloc__Fi; when the block is not null, __11DiseaseBoltsiG6Point3P10GameObjectP5Skill
// is called with (block, the halfword widened to int, rank, the Point3 copy by address, creature, skill). The
// function always returns zero, not the block.
// Halfword stores into the creature (offset = value): 152=-100 148=-100 150=-100 154=0
// Inferred: this launches the projectile at the given point after setting up those creature fields, and keeps no
// handle to it. Unknown: what the creature halfwords are, what the stored values and the angle halfword mean, the
// layout of the skill (not read here) and who owns the projectile afterwards. Nothing here ran on the EE.
struct Point3 { float x, y, z; };
typedef short AliasedShort __attribute__((may_alias));
extern "C" {
void* blockAlloc__Fi(int size);
void projectile_ctor_disease_bolt(void* self, short angle, int rank, Point3 point, void* creature, void* skill) asm("__11DiseaseBoltsiG6Point3P10GameObjectP5Skill");
int cast_disease_bolt(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__16SkillDiseaseBoltiP8CreatureG6Point3s");
}
extern "C" int cast_disease_bolt(void* skill, int rank, void* creature, Point3 point, short angle) {
    unsigned char* bytes = static_cast<unsigned char*>(creature);
    *reinterpret_cast<AliasedShort*>(bytes + 152) = -100;
    *reinterpret_cast<AliasedShort*>(bytes + 148) = -100;
    *reinterpret_cast<AliasedShort*>(bytes + 150) = -100;
    *reinterpret_cast<AliasedShort*>(bytes + 154) = 0;
    void* projectile = blockAlloc__Fi(124);
    if (projectile) projectile_ctor_disease_bolt(projectile, angle, rank, point, creature, skill);
    return 0;
}
