// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002968a8 (8 bytes):
// return the constant 10 for every rank argument. The receiver is not read.
struct SkillDiseaseShield;
extern "C" int get_level_requirement(const SkillDiseaseShield*, int) asm("getLevelRequirement__18SkillDiseaseShieldi");
extern "C" int get_level_requirement(const SkillDiseaseShield*, int) {
    return 10;
}
