// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002a9e58 (40 bytes):
// return (rank - 1) * 9.0f + 102.0 as a single float: the rank is decremented with wraparound, converted to
// float, multiplied by the per-rank step and added to the rank-one cost. The receiver is not read.
struct SkillLightningWeapons;
extern "C" float get_energy_cost(const SkillLightningWeapons*, int) asm("getEnergyCost__21SkillLightningWeaponsi");
extern "C" float get_energy_cost(const SkillLightningWeapons*, int rank) {
    return static_cast<float>(static_cast<int>(static_cast<unsigned>(rank) - 1u)) * 9.0f + 102.0f;
}
