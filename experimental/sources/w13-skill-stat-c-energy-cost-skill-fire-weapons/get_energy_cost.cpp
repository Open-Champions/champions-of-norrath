// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002ab4c8 (40 bytes):
// return (float)(rank - 1) * 10.0 + 195.0, where the factor is the single whose bits are 0x41200000 (an immediate)
// and the added value is a float immediate. The receiver is not read.
struct SkillFireWeapons;
extern "C" float get_energy_cost(const SkillFireWeapons*, int rank) asm("getEnergyCost__16SkillFireWeaponsi");
extern "C" float get_energy_cost(const SkillFireWeapons*, int rank) {
    const float step = __builtin_bit_cast(float, 0x41200000u);
    return static_cast<float>(static_cast<int>(static_cast<unsigned>(rank) - 1u)) * step + 195.0f;
}
