// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002a7480 (8 bytes):
// return the constant 20 for every rank. The receiver and the rank are not read.
struct SkillMinorHealing;
extern "C" int get_level_requirement(const SkillMinorHealing*, int) asm("getLevelRequirement__17SkillMinorHealingi");
extern "C" int get_level_requirement(const SkillMinorHealing*, int) {
    return 20;
}
