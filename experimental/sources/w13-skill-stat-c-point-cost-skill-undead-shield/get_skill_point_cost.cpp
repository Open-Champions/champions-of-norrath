// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002b13d0 (8 bytes):
// return the constant 1 for every rank. The receiver and the rank are not read.
struct SkillUndeadShield;
extern "C" int get_skill_point_cost(const SkillUndeadShield*, int) asm("getSkillPointCost__17SkillUndeadShieldi");
extern "C" int get_skill_point_cost(const SkillUndeadShield*, int) {
    return 1;
}
