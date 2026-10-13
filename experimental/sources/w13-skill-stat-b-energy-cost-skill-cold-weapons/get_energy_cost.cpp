// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002ac470 (40 bytes):
// return (float)(rank - 1) * 9.0 + 137.0, wrapping the subtraction in 32 bits. The receiver is not read.
struct SkillColdWeapons;
extern "C" float get_energy_cost(const SkillColdWeapons*, int rank) asm("getEnergyCost__16SkillColdWeaponsi");
extern "C" float get_energy_cost(const SkillColdWeapons*, int rank) {
    return static_cast<float>(static_cast<int>(static_cast<unsigned>(rank) - 1u)) * 9.0f + 137.0f;
}
