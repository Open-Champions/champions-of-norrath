// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002b6c60 (8 bytes):
// return the constant 5 for every rank. The receiver and the rank are not read.
struct SkillRegeneration;
extern "C" int get_level_requirement(const SkillRegeneration*, int) asm("getLevelRequirement__17SkillRegenerationi");
extern "C" int get_level_requirement(const SkillRegeneration*, int) {
    return 5;
}
