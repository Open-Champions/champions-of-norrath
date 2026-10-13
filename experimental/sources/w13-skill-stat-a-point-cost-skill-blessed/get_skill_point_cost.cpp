// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002a6fc8 (8 bytes):
// return the constant 1 for every rank. The receiver and the rank are not read.
struct SkillBlessed;
extern "C" int get_skill_point_cost(const SkillBlessed*, int) asm("getSkillPointCost__12SkillBlessedi");
extern "C" int get_skill_point_cost(const SkillBlessed*, int) {
    return 1;
}
