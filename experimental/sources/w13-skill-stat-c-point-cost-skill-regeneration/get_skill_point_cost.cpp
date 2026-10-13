// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002b6c58 (8 bytes):
// return the constant 1 for every rank. The receiver and the rank are not read.
struct SkillRegeneration;
extern "C" int get_skill_point_cost(const SkillRegeneration*, int) asm("getSkillPointCost__17SkillRegenerationi");
extern "C" int get_skill_point_cost(const SkillRegeneration*, int) {
    return 1;
}
