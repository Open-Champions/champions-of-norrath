// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002be160 (8 bytes):
// return the constant 1 for every rank. The receiver and the rank are not read.
struct SkillShockOfFrost;
extern "C" int get_level_requirement(const SkillShockOfFrost*, int) asm("getLevelRequirement__17SkillShockOfFrosti");
extern "C" int get_level_requirement(const SkillShockOfFrost*, int) {
    return 1;
}
