// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002a5108 (8 bytes):
// return the constant 1 for every rank. The receiver and the rank are not read.
struct SkillRiposte;
extern "C" int get_skill_point_cost(const SkillRiposte*, int) asm("getSkillPointCost__12SkillRipostei");
extern "C" int get_skill_point_cost(const SkillRiposte*, int) {
    return 1;
}
