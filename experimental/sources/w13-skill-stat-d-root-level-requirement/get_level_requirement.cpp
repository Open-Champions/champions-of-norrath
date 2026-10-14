// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002ad050 (8 bytes):
// return the constant 25 for every rank argument. The receiver is not read.
struct SkillRoot;
extern "C" int get_level_requirement(const SkillRoot*, int) asm("getLevelRequirement__9SkillRooti");
extern "C" int get_level_requirement(const SkillRoot*, int) {
    return 25;
}
