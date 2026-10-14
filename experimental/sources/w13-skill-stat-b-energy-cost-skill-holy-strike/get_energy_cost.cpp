// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002b2e20 (40 bytes):
// return (float)(rank - 1) * 3.0 + 7.0, wrapping the subtraction in 32 bits. The receiver is not read.
struct SkillHolyStrike;
extern "C" float get_energy_cost(const SkillHolyStrike*, int rank) asm("getEnergyCost__15SkillHolyStrikei");
extern "C" float get_energy_cost(const SkillHolyStrike*, int rank) {
    return static_cast<float>(static_cast<int>(static_cast<unsigned>(rank) - 1u)) * 3.0f + 7.0f;
}
