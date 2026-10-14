// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00313248 (8 bytes):
// return the signed integer argument unchanged; the receiver is not read.
struct Skill;
extern "C" int get_skill_point_cost(const Skill*, int rank) asm("getSkillPointCost__5Skilli");
extern "C" int get_skill_point_cost(const Skill*, int rank) {
    return rank;
}
