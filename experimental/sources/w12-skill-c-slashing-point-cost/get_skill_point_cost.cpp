// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00313c28 (8 bytes):
// return the integer argument unchanged. The receiver is not read.
struct SkillSlashing;
extern "C" int get_skill_point_cost(const SkillSlashing*, int) asm("getSkillPointCost__13SkillSlashingi");
extern "C" int get_skill_point_cost(const SkillSlashing*, int rank) {
    return rank;
}
