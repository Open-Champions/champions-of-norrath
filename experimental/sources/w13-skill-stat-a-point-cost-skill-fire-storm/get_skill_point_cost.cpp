// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x0029d230 (8 bytes):
// return the constant 1 for every rank. The receiver and the rank are not read.
struct SkillFireStorm;
extern "C" int get_skill_point_cost(const SkillFireStorm*, int) asm("getSkillPointCost__14SkillFireStormi");
extern "C" int get_skill_point_cost(const SkillFireStorm*, int) {
    return 1;
}
