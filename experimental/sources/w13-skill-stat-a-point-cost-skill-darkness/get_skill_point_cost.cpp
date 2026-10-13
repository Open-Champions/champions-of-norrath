// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002982f0 (12 bytes):
// return three times the rank (wrapping, low 32 bits). The receiver is not read.
struct SkillDarkness;
extern "C" int get_skill_point_cost(const SkillDarkness*, int rank) asm("getSkillPointCost__13SkillDarknessi");
extern "C" int get_skill_point_cost(const SkillDarkness*, int rank) {
    return static_cast<int>(static_cast<unsigned>(rank) * 3u);
}
