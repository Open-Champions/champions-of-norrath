// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002b6af8 (8 bytes):
// return the constant 1 for every rank. The receiver and the rank are not read.
struct SkillConeOfFire;
extern "C" int get_skill_point_cost(const SkillConeOfFire*, int) asm("getSkillPointCost__15SkillConeOfFirei");
extern "C" int get_skill_point_cost(const SkillConeOfFire*, int) {
    return 1;
}
