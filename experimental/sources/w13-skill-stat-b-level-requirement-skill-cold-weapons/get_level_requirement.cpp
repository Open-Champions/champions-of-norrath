// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002ac468 (8 bytes):
// return the constant 15 for every rank. The receiver and the rank are not read.
struct SkillColdWeapons;
extern "C" int get_level_requirement(const SkillColdWeapons*, int) asm("getLevelRequirement__16SkillColdWeaponsi");
extern "C" int get_level_requirement(const SkillColdWeapons*, int) {
    return 15;
}
