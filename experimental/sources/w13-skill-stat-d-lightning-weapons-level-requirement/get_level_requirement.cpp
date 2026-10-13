// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002a9e50 (8 bytes):
// return the constant 12 for every rank argument. The receiver is not read.
struct SkillLightningWeapons;
extern "C" int get_level_requirement(const SkillLightningWeapons*, int) asm("getLevelRequirement__21SkillLightningWeaponsi");
extern "C" int get_level_requirement(const SkillLightningWeapons*, int) {
    return 12;
}
