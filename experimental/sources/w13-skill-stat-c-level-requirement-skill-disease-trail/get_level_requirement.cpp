// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00295538 (8 bytes):
// return the constant 8 for every rank. The receiver and the rank are not read.
struct SkillDiseaseTrail;
extern "C" int get_level_requirement(const SkillDiseaseTrail*, int) asm("getLevelRequirement__17SkillDiseaseTraili");
extern "C" int get_level_requirement(const SkillDiseaseTrail*, int) {
    return 8;
}
