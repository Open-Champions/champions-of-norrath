// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x0029a9d8 (8 bytes):
// return the constant 18 for every rank. The receiver and the rank are not read.
struct SkillCyclone;
extern "C" int get_level_requirement(const SkillCyclone*, int) asm("getLevelRequirement__12SkillCyclonei");
extern "C" int get_level_requirement(const SkillCyclone*, int) {
    return 18;
}
