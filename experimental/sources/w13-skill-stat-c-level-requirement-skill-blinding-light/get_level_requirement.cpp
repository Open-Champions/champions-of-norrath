// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002ba180 (8 bytes):
// return the constant 15 for every rank. The receiver and the rank are not read.
struct SkillBlindingLight;
extern "C" int get_level_requirement(const SkillBlindingLight*, int) asm("getLevelRequirement__18SkillBlindingLighti");
extern "C" int get_level_requirement(const SkillBlindingLight*, int) {
    return 15;
}
