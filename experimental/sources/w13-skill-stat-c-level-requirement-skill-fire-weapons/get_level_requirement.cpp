// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002ab4c0 (8 bytes):
// return the constant 20 for every rank. The receiver and the rank are not read.
struct SkillFireWeapons;
extern "C" int get_level_requirement(const SkillFireWeapons*, int) asm("getLevelRequirement__16SkillFireWeaponsi");
extern "C" int get_level_requirement(const SkillFireWeapons*, int) {
    return 20;
}
