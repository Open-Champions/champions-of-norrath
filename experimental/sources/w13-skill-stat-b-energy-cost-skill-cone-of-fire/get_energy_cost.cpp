// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002b6b08 (36 bytes):
// return (float)(rank - 1) * step + 42.0, with step the single whose bits are 0x40000008 (read from the image as a 4-byte
// small-data word, owner unknown). The receiver is not read.
struct SkillConeOfFire;
extern "C" float get_energy_cost(const SkillConeOfFire*, int rank) asm("getEnergyCost__15SkillConeOfFirei");
extern "C" float get_energy_cost(const SkillConeOfFire*, int rank) {
    const float step = __builtin_bit_cast(float, 0x40000008u);
    return static_cast<float>(static_cast<int>(static_cast<unsigned>(rank) - 1u)) * step + 42.0f;
}
