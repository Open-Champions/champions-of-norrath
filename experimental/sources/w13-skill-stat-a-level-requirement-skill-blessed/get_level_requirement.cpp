// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002a6fd0 (8 bytes):
// return the constant 8 for every rank. The receiver and the rank are not read.
struct SkillBlessed;
extern "C" int get_level_requirement(const SkillBlessed*, int) asm("getLevelRequirement__12SkillBlessedi");
extern "C" int get_level_requirement(const SkillBlessed*, int) {
    return 8;
}
