// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00313b40 (8 bytes):
// return the low 32 bits of the argument shifted left by two bits. The receiver is not read.
struct SkillBlunt;
extern "C" unsigned int get_level_requirement(const SkillBlunt*, unsigned int) asm("getLevelRequirement__10SkillBlunti");
extern "C" unsigned int get_level_requirement(const SkillBlunt*, unsigned int rank) {
    return rank << 2;
}
