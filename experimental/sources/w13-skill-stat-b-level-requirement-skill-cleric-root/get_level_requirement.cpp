// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002c6318 (8 bytes):
// return the constant 20 for every rank. The receiver and the rank are not read.
struct SkillClericRoot;
extern "C" int get_level_requirement(const SkillClericRoot*, int) asm("getLevelRequirement__15SkillClericRooti");
extern "C" int get_level_requirement(const SkillClericRoot*, int) {
    return 20;
}
