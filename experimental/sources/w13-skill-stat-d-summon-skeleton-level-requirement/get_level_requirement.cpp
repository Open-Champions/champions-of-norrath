// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002a9178 (8 bytes):
// return the constant 15 for every rank argument. The receiver is not read.
struct SkillSummonSkeleton;
extern "C" int get_level_requirement(const SkillSummonSkeleton*, int) asm("getLevelRequirement__19SkillSummonSkeletoni");
extern "C" int get_level_requirement(const SkillSummonSkeleton*, int) {
    return 15;
}
