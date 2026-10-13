// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002b9480 (8 bytes):
// return the constant 10 for every rank. The receiver and the rank are not read.
struct SkillWizardBeam;
extern "C" int get_level_requirement(const SkillWizardBeam*, int) asm("getLevelRequirement__15SkillWizardBeami");
extern "C" int get_level_requirement(const SkillWizardBeam*, int) {
    return 10;
}
