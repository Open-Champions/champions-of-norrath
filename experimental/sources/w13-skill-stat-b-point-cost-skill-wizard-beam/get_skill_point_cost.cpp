// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002b9478 (8 bytes):
// return the constant 1 for every rank. The receiver and the rank are not read.
struct SkillWizardBeam;
extern "C" int get_skill_point_cost(const SkillWizardBeam*, int) asm("getSkillPointCost__15SkillWizardBeami");
extern "C" int get_skill_point_cost(const SkillWizardBeam*, int) {
    return 1;
}
