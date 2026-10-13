// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002b2e18 (8 bytes):
// return the constant 1 for every rank. The receiver and the rank are not read.
struct SkillHolyStrike;
extern "C" int get_level_requirement(const SkillHolyStrike*, int) asm("getLevelRequirement__15SkillHolyStrikei");
extern "C" int get_level_requirement(const SkillHolyStrike*, int) {
    return 1;
}
