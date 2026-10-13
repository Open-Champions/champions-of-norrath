// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002c6f90 (8 bytes):
// return the constant 25 for every rank. The receiver and the rank are not read.
struct SkillEntangle;
extern "C" int get_level_requirement(const SkillEntangle*, int) asm("getLevelRequirement__13SkillEntanglei");
extern "C" int get_level_requirement(const SkillEntangle*, int) {
    return 25;
}
