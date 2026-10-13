// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002ba188 (36 bytes):
// return (float)(rank - 1) * 2.999997854232788 + 45.0, where the factor is the single whose bits are 0x403ffff7 (read from the image as a 4-byte small-data constant, owner unknown)
// and the added value is a float immediate. The receiver is not read.
struct SkillBlindingLight;
extern "C" float get_energy_cost(const SkillBlindingLight*, int rank) asm("getEnergyCost__18SkillBlindingLighti");
extern "C" float get_energy_cost(const SkillBlindingLight*, int rank) {
    const float step = __builtin_bit_cast(float, 0x403ffff7u);
    return static_cast<float>(static_cast<int>(static_cast<unsigned>(rank) - 1u)) * step + 45.0f;
}
