// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002c7d58 (8 bytes):
// return the constant 10 for every rank argument. The receiver is not read.
struct SkillRangerCriticalHit;
extern "C" int get_level_requirement(const SkillRangerCriticalHit*, int) asm("getLevelRequirement__22SkillRangerCriticalHiti");
extern "C" int get_level_requirement(const SkillRangerCriticalHit*, int) {
    return 10;
}
