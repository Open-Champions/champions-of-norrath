// Observed cast__15SkillHolyStrikeiP8CreatureG6Point3s at 0x002b2d50, 192 bytes in .text. The fifth argument is a signed halfword
// (the trailing "s" of the mangled name).
// Steps: the by-value Point3 is copied to the frame; halfwords are written into the creature (listed below);
// 128 bytes are allocated with blockAlloc__Fi; when the block is not null, __10HolyStrikesiG6Point3P10GameObjectP5Skill
// is called with (block, the halfword widened to int, rank, the Point3 copy by address, creature, skill). The
// function always returns zero, not the block.
// Halfword stores into the creature (offset = value): 144=135 146=64 152=135 154=64 140=255 142=200 148=255 150=200
// Inferred: this launches the projectile at the given point after setting up those creature fields, and keeps no
// handle to it. Unknown: what the creature halfwords are, what the stored values and the angle halfword mean, the
// layout of the skill (not read here) and who owns the projectile afterwards. Nothing here ran on the EE.
struct Point3 { float x, y, z; };
typedef short AliasedShort __attribute__((may_alias));
extern "C" {
void* blockAlloc__Fi(int size);
void projectile_ctor_holy_strike(void* self, short angle, int rank, Point3 point, void* creature, void* skill) asm("__10HolyStrikesiG6Point3P10GameObjectP5Skill");
int cast_holy_strike(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__15SkillHolyStrikeiP8CreatureG6Point3s");
}
extern "C" int cast_holy_strike(void* skill, int rank, void* creature, Point3 point, short angle) {
    unsigned char* bytes = static_cast<unsigned char*>(creature);
    *reinterpret_cast<AliasedShort*>(bytes + 144) = 135;
    *reinterpret_cast<AliasedShort*>(bytes + 146) = 64;
    *reinterpret_cast<AliasedShort*>(bytes + 152) = 135;
    *reinterpret_cast<AliasedShort*>(bytes + 154) = 64;
    *reinterpret_cast<AliasedShort*>(bytes + 140) = 255;
    *reinterpret_cast<AliasedShort*>(bytes + 142) = 200;
    *reinterpret_cast<AliasedShort*>(bytes + 148) = 255;
    *reinterpret_cast<AliasedShort*>(bytes + 150) = 200;
    void* projectile = blockAlloc__Fi(128);
    if (projectile) projectile_ctor_holy_strike(projectile, angle, rank, point, creature, skill);
    return 0;
}
