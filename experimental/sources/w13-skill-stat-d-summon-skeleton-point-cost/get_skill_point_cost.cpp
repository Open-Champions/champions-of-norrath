// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002a9170 (8 bytes):
// return the constant 1 for every rank argument. The receiver is not read.
struct SkillSummonSkeleton;
extern "C" int get_skill_point_cost(const SkillSummonSkeleton*, int) asm("getSkillPointCost__19SkillSummonSkeletoni");
extern "C" int get_skill_point_cost(const SkillSummonSkeleton*, int) {
    return 1;
}
