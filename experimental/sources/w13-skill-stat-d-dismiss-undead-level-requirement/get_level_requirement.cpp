// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002bbd28 (8 bytes):
// return the constant 25 for every rank argument. The receiver is not read.
struct SkillDismissUndead;
extern "C" int get_level_requirement(const SkillDismissUndead*, int) asm("getLevelRequirement__18SkillDismissUndeadi");
extern "C" int get_level_requirement(const SkillDismissUndead*, int) {
    return 25;
}
