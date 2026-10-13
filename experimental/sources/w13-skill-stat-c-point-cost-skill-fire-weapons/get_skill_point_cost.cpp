// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002ab4b8 (8 bytes):
// return the constant 1 for every rank. The receiver and the rank are not read.
struct SkillFireWeapons;
extern "C" int get_skill_point_cost(const SkillFireWeapons*, int) asm("getSkillPointCost__16SkillFireWeaponsi");
extern "C" int get_skill_point_cost(const SkillFireWeapons*, int) {
    return 1;
}
