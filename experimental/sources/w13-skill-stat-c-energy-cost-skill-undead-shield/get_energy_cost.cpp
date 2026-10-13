// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002b13e0 (36 bytes):
// return (float)(rank - 1) * 4.000024795532227 + 41.0, where the factor is the single whose bits are 0x40800034 (read from the image as a 4-byte small-data constant, owner unknown)
// and the added value is a float immediate. The receiver is not read.
struct SkillUndeadShield;
extern "C" float get_energy_cost(const SkillUndeadShield*, int rank) asm("getEnergyCost__17SkillUndeadShieldi");
extern "C" float get_energy_cost(const SkillUndeadShield*, int rank) {
    const float step = __builtin_bit_cast(float, 0x40800034u);
    return static_cast<float>(static_cast<int>(static_cast<unsigned>(rank) - 1u)) * step + 41.0f;
}
