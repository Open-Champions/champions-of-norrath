// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002b5810 (8 bytes):
// return the constant 10 for every rank argument. The receiver is not read.
struct SkillHammerOfWrath;
extern "C" int get_level_requirement(const SkillHammerOfWrath*, int) asm("getLevelRequirement__18SkillHammerOfWrathi");
extern "C" int get_level_requirement(const SkillHammerOfWrath*, int) {
    return 10;
}
