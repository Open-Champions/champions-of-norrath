// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002ba730 (8 bytes):
// return the constant 1 for every rank argument. The receiver is not read.
struct SkillRepulseUndead;
extern "C" int get_skill_point_cost(const SkillRepulseUndead*, int) asm("getSkillPointCost__18SkillRepulseUndeadi");
extern "C" int get_skill_point_cost(const SkillRepulseUndead*, int) {
    return 1;
}
