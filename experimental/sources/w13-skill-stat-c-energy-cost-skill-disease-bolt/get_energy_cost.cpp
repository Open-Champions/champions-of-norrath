// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00293878 (36 bytes):
// return (float)(rank - 1) * 2.0000128746032715 + 9.0, where the factor is the single whose bits are 0x40000036 (read from the image as a 4-byte small-data constant, owner unknown)
// and the added value is a float immediate. The receiver is not read.
struct SkillDiseaseBolt;
extern "C" float get_energy_cost(const SkillDiseaseBolt*, int rank) asm("getEnergyCost__16SkillDiseaseBolti");
extern "C" float get_energy_cost(const SkillDiseaseBolt*, int rank) {
    const float step = __builtin_bit_cast(float, 0x40000036u);
    return static_cast<float>(static_cast<int>(static_cast<unsigned>(rank) - 1u)) * step + 9.0f;
}
