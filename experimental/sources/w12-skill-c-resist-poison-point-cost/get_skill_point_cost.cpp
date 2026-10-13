// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00313d10 (8 bytes):
// return the integer argument unchanged. The receiver is not read.
struct SkillResistPoison;
extern "C" int get_skill_point_cost(const SkillResistPoison*, int) asm("getSkillPointCost__17SkillResistPoisoni");
extern "C" int get_skill_point_cost(const SkillResistPoison*, int rank) {
    return rank;
}
