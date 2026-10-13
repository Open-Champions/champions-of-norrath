// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002ac460 (8 bytes):
// return the constant 1 for every rank. The receiver and the rank are not read.
struct SkillColdWeapons;
extern "C" int get_skill_point_cost(const SkillColdWeapons*, int) asm("getSkillPointCost__16SkillColdWeaponsi");
extern "C" int get_skill_point_cost(const SkillColdWeapons*, int) {
    return 1;
}
