// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002a5f40 (8 bytes):
// return the constant 10 for every rank argument. The receiver is not read.
struct SkillBash;
extern "C" int get_level_requirement(const SkillBash*, int) asm("getLevelRequirement__9SkillBashi");
extern "C" int get_level_requirement(const SkillBash*, int) {
    return 10;
}
