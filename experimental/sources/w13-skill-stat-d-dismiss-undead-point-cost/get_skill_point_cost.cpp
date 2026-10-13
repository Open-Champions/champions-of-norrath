// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002bbd20 (8 bytes):
// return the constant 1 for every rank argument. The receiver is not read.
struct SkillDismissUndead;
extern "C" int get_skill_point_cost(const SkillDismissUndead*, int) asm("getSkillPointCost__18SkillDismissUndeadi");
extern "C" int get_skill_point_cost(const SkillDismissUndead*, int) {
    return 1;
}
