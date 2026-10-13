// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002a9e48 (8 bytes):
// return the constant 1 for every rank argument. The receiver is not read.
struct SkillLightningWeapons;
extern "C" int get_skill_point_cost(const SkillLightningWeapons*, int) asm("getSkillPointCost__21SkillLightningWeaponsi");
extern "C" int get_skill_point_cost(const SkillLightningWeapons*, int) {
    return 1;
}
