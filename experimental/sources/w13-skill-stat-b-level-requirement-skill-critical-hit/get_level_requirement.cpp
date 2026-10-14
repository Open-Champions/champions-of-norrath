// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002addb0 (8 bytes):
// return the constant 8 for every rank. The receiver and the rank are not read.
struct SkillCriticalHit;
extern "C" int get_level_requirement(const SkillCriticalHit*, int) asm("getLevelRequirement__16SkillCriticalHiti");
extern "C" int get_level_requirement(const SkillCriticalHit*, int) {
    return 8;
}
