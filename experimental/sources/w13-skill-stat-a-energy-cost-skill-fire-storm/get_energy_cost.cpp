// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x0029d240 (36 bytes):
// return (float)(rank - 1) * 6.0000048 + 120.0, with the factor the single whose bits are 0x40c0000a (read from the image
// as a 4-byte small-data constant, owner unknown). The receiver is not read.
struct SkillFireStorm;
extern "C" float get_energy_cost(const SkillFireStorm*, int rank) asm("getEnergyCost__14SkillFireStormi");
extern "C" float get_energy_cost(const SkillFireStorm*, int rank) {
    const float step = __builtin_bit_cast(float, 0x40c0000au);
    return static_cast<float>(static_cast<int>(static_cast<unsigned>(rank) - 1u)) * step + 120.0f;
}
