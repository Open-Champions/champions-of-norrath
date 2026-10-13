// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002968b0 (36 bytes):
// return (rank - 1) * 0x1.000046p+2f + 43.0 as a single float: the rank is decremented with wraparound, converted to
// float, multiplied by the per-rank step and added to the rank-one cost. The receiver is not read.
struct SkillDiseaseShield;
extern "C" float get_energy_cost(const SkillDiseaseShield*, int) asm("getEnergyCost__18SkillDiseaseShieldi");
extern "C" float get_energy_cost(const SkillDiseaseShield*, int rank) {
    return static_cast<float>(static_cast<int>(static_cast<unsigned>(rank) - 1u)) * 0x1.000046p+2f + 43.0f;
}
