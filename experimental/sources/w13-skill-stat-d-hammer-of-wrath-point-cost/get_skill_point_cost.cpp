// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002b5808 (8 bytes):
// return the constant 1 for every rank argument. The receiver is not read.
struct SkillHammerOfWrath;
extern "C" int get_skill_point_cost(const SkillHammerOfWrath*, int) asm("getSkillPointCost__18SkillHammerOfWrathi");
extern "C" int get_skill_point_cost(const SkillHammerOfWrath*, int) {
    return 1;
}
