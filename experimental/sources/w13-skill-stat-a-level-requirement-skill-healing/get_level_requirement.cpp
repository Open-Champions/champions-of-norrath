// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002af368 (8 bytes):
// return the constant 8 for every rank. The receiver and the rank are not read.
struct SkillHealing;
extern "C" int get_level_requirement(const SkillHealing*, int) asm("getLevelRequirement__12SkillHealingi");
extern "C" int get_level_requirement(const SkillHealing*, int) {
    return 8;
}
