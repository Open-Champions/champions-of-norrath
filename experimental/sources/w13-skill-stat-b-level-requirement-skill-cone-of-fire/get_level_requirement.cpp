// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002b6b00 (8 bytes):
// return the constant 28 for every rank. The receiver and the rank are not read.
struct SkillConeOfFire;
extern "C" int get_level_requirement(const SkillConeOfFire*, int) asm("getLevelRequirement__15SkillConeOfFirei");
extern "C" int get_level_requirement(const SkillConeOfFire*, int) {
    return 28;
}
