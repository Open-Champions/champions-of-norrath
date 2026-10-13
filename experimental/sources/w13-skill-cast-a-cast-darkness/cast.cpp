// Observed cast__13SkillDarknessiP8CreatureG6Point3s at 0x00297658, 312 bytes in .text. The fifth argument is a
// signed halfword (the trailing "s" of the mangled name), used as an integer angle. The incoming Point3 is copied to
// the frame and not read.
// Steps, in order: (1) a direction d = (icos(angle), isin(angle), 0) is computed and each component is multiplied by
// the float 373.0 (a literal the original loads from its small-data area); (2) 176 bytes are allocated with
// blockAlloc__Fi; (3) when the block is not null a start point s is built from the three floats at creature+16: s.x =
// x + 0.0, s.y = y, s.z = z + 36.0 (the x sum is a real addition of zero, which turns a negative zero into positive
// zero); (4) __18DarknessProjectileG6Point3T1P5SkilliP10GameObject is called with (block, s by address, d by address,
// skill, rank, creature). The function always returns zero, not the block.
// Inferred: the projectile starts 36 units above the creature's position and is given a velocity of 373 units along
// the heading, with no vertical part. Unknown: what the three floats at creature+16 are called (they read as a
// position), the unit of the angle, the layout of the skill (not read here) and who owns the projectile.
// Nothing here ran on the EE.
struct Point3 { float x, y, z; };
typedef float AliasedFloat __attribute__((may_alias));
extern "C" {
void* blockAlloc__Fi(int size);
float icos__Fi(int angle);
float isin__Fi(int angle);
void projectile_ctor_darkness(void* self, Point3 start, Point3 direction, void* skill, int rank, void* creature) asm("__18DarknessProjectileG6Point3T1P5SkilliP10GameObject");
int cast_darkness(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__13SkillDarknessiP8CreatureG6Point3s");
}
extern "C" int cast_darkness(void* skill, int rank, void* creature, Point3, short angle) {
    Point3 d;
    d.x = icos__Fi(angle);
    d.y = isin__Fi(angle);
    d.z = 0.0f;
    d.x *= 373.0f;
    d.y *= 373.0f;
    d.z *= 373.0f;
    void* projectile = blockAlloc__Fi(176);
    if (projectile) {
        const AliasedFloat* position = reinterpret_cast<AliasedFloat*>(static_cast<unsigned char*>(creature) + 16);
        Point3 s;
        s.x = position[0] + 0.0f;
        s.y = position[1];
        s.z = position[2] + 36.0f;
        projectile_ctor_darkness(projectile, s, d, skill, rank, creature);
    }
    return 0;
}
