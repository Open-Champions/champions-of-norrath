// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002c8b90 (8 bytes):
// return the constant 5 for every rank. The receiver and the rank are not read.
struct SkillShieldBash;
extern "C" int get_level_requirement(const SkillShieldBash*, int) asm("getLevelRequirement__15SkillShieldBashi");
extern "C" int get_level_requirement(const SkillShieldBash*, int) {
    return 5;
}
