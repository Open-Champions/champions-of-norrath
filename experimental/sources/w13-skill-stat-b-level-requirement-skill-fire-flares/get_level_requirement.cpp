// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002a4970 (8 bytes):
// return the constant 20 for every rank. The receiver and the rank are not read.
struct SkillFireFlares;
extern "C" int get_level_requirement(const SkillFireFlares*, int) asm("getLevelRequirement__15SkillFireFlaresi");
extern "C" int get_level_requirement(const SkillFireFlares*, int) {
    return 20;
}
