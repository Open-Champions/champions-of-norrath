// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002ba178 (8 bytes):
// return the constant 1 for every rank argument. The receiver is not read.
struct SkillBlindingLight;
extern "C" int get_skill_point_cost(const SkillBlindingLight*, int) asm("getSkillPointCost__18SkillBlindingLighti");
extern "C" int get_skill_point_cost(const SkillBlindingLight*, int) {
    return 1;
}
