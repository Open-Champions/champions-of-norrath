// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002be168 (36 bytes):
// return (float)(rank - 1) * 3.000032901763916 + 7.0, where the factor is the single whose bits are 0x4040008a (read from the image as a 4-byte small-data constant, owner unknown)
// and the added value is a float immediate. The receiver is not read.
struct SkillShockOfFrost;
extern "C" float get_energy_cost(const SkillShockOfFrost*, int rank) asm("getEnergyCost__17SkillShockOfFrosti");
extern "C" float get_energy_cost(const SkillShockOfFrost*, int rank) {
    const float step = __builtin_bit_cast(float, 0x4040008au);
    return static_cast<float>(static_cast<int>(static_cast<unsigned>(rank) - 1u)) * step + 7.0f;
}
