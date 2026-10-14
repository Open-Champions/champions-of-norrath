// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002a4978 (40 bytes):
// return (float)(rank - 1) * 6.0 + 122.0, wrapping the subtraction in 32 bits. The receiver is not read.
struct SkillFireFlares;
extern "C" float get_energy_cost(const SkillFireFlares*, int rank) asm("getEnergyCost__15SkillFireFlaresi");
extern "C" float get_energy_cost(const SkillFireFlares*, int rank) {
    return static_cast<float>(static_cast<int>(static_cast<unsigned>(rank) - 1u)) * 6.0f + 122.0f;
}
