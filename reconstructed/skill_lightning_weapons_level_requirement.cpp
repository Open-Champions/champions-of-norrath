// Independently authored from the behavior of the hash-pinned Norrath ELF.
// Original symbol: getLevelRequirement__21SkillLightningWeaponsi
// Address: 0x002a9e50; complete symbol extent: 8 bytes.
// This function returns 12 without reading either input or modifying memory.
// The retained name suggests a level requirement; gameplay use is not yet run.
// The opaque receiver and integer argument reflect the retained member signature.
// This C entry point is a comparison adapter, not the original C++ class ABI.
struct SkillLightningWeapons;

static_assert(sizeof(int) == 4, "The comparison requires a 32-bit int.");

extern "C" int norrath_lightning_weapons_level_requirement(
    const SkillLightningWeapons*, int) {
    return 12;
}
