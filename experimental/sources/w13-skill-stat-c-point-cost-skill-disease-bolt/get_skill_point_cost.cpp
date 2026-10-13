// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00293868 (8 bytes):
// return the constant 1 for every rank. The receiver and the rank are not read.
struct SkillDiseaseBolt;
extern "C" int get_skill_point_cost(const SkillDiseaseBolt*, int) asm("getSkillPointCost__16SkillDiseaseBolti");
extern "C" int get_skill_point_cost(const SkillDiseaseBolt*, int) {
    return 1;
}
