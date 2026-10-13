// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00298300 (8 bytes):
// return the rank shifted left by two (the low 32 bits). The receiver is not read.
struct SkillDarkness;
extern "C" int get_level_requirement(const SkillDarkness*, int rank) asm("getLevelRequirement__13SkillDarknessi");
extern "C" int get_level_requirement(const SkillDarkness*, int rank) {
    return static_cast<int>(static_cast<unsigned>(rank) << 2);
}
