// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00293870 (8 bytes):
// return the constant 3 for every rank. The receiver and the rank are not read.
struct SkillDiseaseBolt;
extern "C" int get_level_requirement(const SkillDiseaseBolt*, int) asm("getLevelRequirement__16SkillDiseaseBolti");
extern "C" int get_level_requirement(const SkillDiseaseBolt*, int) {
    return 3;
}
