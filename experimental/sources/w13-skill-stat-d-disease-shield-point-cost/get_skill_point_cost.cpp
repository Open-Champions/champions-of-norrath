// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002968a0 (8 bytes):
// return the constant 1 for every rank argument. The receiver is not read.
struct SkillDiseaseShield;
extern "C" int get_skill_point_cost(const SkillDiseaseShield*, int) asm("getSkillPointCost__18SkillDiseaseShieldi");
extern "C" int get_skill_point_cost(const SkillDiseaseShield*, int) {
    return 1;
}
