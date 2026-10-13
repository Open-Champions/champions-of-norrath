// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002bf230 (8 bytes):
// return the constant 1 for every rank. The receiver and the rank are not read.
struct SkillPoisonRain;
extern "C" int get_skill_point_cost(const SkillPoisonRain*, int) asm("getSkillPointCost__15SkillPoisonRaini");
extern "C" int get_skill_point_cost(const SkillPoisonRain*, int) {
    return 1;
}
