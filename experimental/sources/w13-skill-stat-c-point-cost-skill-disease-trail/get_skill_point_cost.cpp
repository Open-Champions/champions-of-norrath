// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00295530 (8 bytes):
// return the constant 1 for every rank. The receiver and the rank are not read.
struct SkillDiseaseTrail;
extern "C" int get_skill_point_cost(const SkillDiseaseTrail*, int) asm("getSkillPointCost__17SkillDiseaseTraili");
extern "C" int get_skill_point_cost(const SkillDiseaseTrail*, int) {
    return 1;
}
