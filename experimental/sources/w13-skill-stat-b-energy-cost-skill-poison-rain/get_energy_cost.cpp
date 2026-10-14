// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002bf240 (40 bytes):
// return (float)(rank - 1) * 12.0 + 222.0, wrapping the subtraction in 32 bits. The receiver is not read.
struct SkillPoisonRain;
extern "C" float get_energy_cost(const SkillPoisonRain*, int rank) asm("getEnergyCost__15SkillPoisonRaini");
extern "C" float get_energy_cost(const SkillPoisonRain*, int rank) {
    return static_cast<float>(static_cast<int>(static_cast<unsigned>(rank) - 1u)) * 12.0f + 222.0f;
}
