// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00295540 (36 bytes):
// return (float)(rank - 1) * 2.9999969005584717 + 28.0, where the factor is the single whose bits are 0x403ffff3 (read from the image as a 4-byte small-data constant, owner unknown)
// and the added value is a float immediate. The receiver is not read.
struct SkillDiseaseTrail;
extern "C" float get_energy_cost(const SkillDiseaseTrail*, int rank) asm("getEnergyCost__17SkillDiseaseTraili");
extern "C" float get_energy_cost(const SkillDiseaseTrail*, int rank) {
    const float step = __builtin_bit_cast(float, 0x403ffff3u);
    return static_cast<float>(static_cast<int>(static_cast<unsigned>(rank) - 1u)) * step + 28.0f;
}
