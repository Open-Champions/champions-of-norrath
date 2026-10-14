// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002b3290 (8 bytes):
// return the constant 15 for every rank. The receiver and the rank are not read.
struct SkillShockOfFire;
extern "C" int get_level_requirement(const SkillShockOfFire*, int) asm("getLevelRequirement__16SkillShockOfFirei");
extern "C" int get_level_requirement(const SkillShockOfFire*, int) {
    return 15;
}
