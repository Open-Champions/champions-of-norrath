// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002a4968 (8 bytes):
// return the constant 1 for every rank. The receiver and the rank are not read.
struct SkillFireFlares;
extern "C" int get_skill_point_cost(const SkillFireFlares*, int) asm("getSkillPointCost__15SkillFireFlaresi");
extern "C" int get_skill_point_cost(const SkillFireFlares*, int) {
    return 1;
}
