// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002b9488 (36 bytes):
// return (float)(rank - 1) * step + 22.0, with step the single whose bits are 0x40000021 (read from the image as a 4-byte
// small-data word, owner unknown). The receiver is not read.
struct SkillWizardBeam;
extern "C" float get_energy_cost(const SkillWizardBeam*, int rank) asm("getEnergyCost__15SkillWizardBeami");
extern "C" float get_energy_cost(const SkillWizardBeam*, int rank) {
    const float step = __builtin_bit_cast(float, 0x40000021u);
    return static_cast<float>(static_cast<int>(static_cast<unsigned>(rank) - 1u)) * step + 22.0f;
}
