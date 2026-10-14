// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00313c30 (8 bytes):
// return the low 32 bits of the argument shifted left by two bits. The receiver is not read.
struct SkillSlashing;
extern "C" unsigned int get_level_requirement(const SkillSlashing*, unsigned int) asm("getLevelRequirement__13SkillSlashingi");
extern "C" unsigned int get_level_requirement(const SkillSlashing*, unsigned int rank) {
    return rank << 2;
}
