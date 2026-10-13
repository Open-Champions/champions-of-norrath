// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00313b38 (8 bytes):
// return the integer argument unchanged. The receiver is not read.
struct SkillBlunt;
extern "C" int get_skill_point_cost(const SkillBlunt*, int) asm("getSkillPointCost__10SkillBlunti");
extern "C" int get_skill_point_cost(const SkillBlunt*, int rank) {
    return rank;
}
