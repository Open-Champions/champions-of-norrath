// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002ba738 (8 bytes):
// return the constant 15 for every rank argument. The receiver is not read.
struct SkillRepulseUndead;
extern "C" int get_level_requirement(const SkillRepulseUndead*, int) asm("getLevelRequirement__18SkillRepulseUndeadi");
extern "C" int get_level_requirement(const SkillRepulseUndead*, int) {
    return 15;
}
