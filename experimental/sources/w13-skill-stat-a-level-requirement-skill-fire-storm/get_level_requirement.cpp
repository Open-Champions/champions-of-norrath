// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x0029d238 (8 bytes):
// return the constant 20 for every rank. The receiver and the rank are not read.
struct SkillFireStorm;
extern "C" int get_level_requirement(const SkillFireStorm*, int) asm("getLevelRequirement__14SkillFireStormi");
extern "C" int get_level_requirement(const SkillFireStorm*, int) {
    return 20;
}
