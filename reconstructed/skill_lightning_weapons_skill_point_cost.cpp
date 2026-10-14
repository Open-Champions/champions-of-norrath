// Independently authored from the behavior of the hash-pinned Norrath ELF.
// Original symbol: getSkillPointCost__21SkillLightningWeaponsi
// Address: 0x002a9e48; complete symbol extent: 8 bytes.
// This function returns 1 without reading either input or modifying memory.
// Its retained name suggests a skill-point cost; gameplay use is not yet run.
// The opaque receiver and integer argument reflect the retained member signature.
// This C entry point is a comparison adapter, not the original C++ class ABI.
struct SkillLightningWeapons;

static_assert(sizeof(int) == 4, "The comparison requires a 32-bit int.");

extern "C" int norrath_lightning_weapons_skill_point_cost(
    const SkillLightningWeapons*, int) {
    return 1;
}
