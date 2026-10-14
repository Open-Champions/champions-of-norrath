// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002a9180 (40 bytes):
// return (rank - 1) * 5.0f + 81.0 as a single float: the rank is decremented with wraparound, converted to
// float, multiplied by the per-rank step and added to the rank-one cost. The receiver is not read.
struct SkillSummonSkeleton;
extern "C" float get_energy_cost(const SkillSummonSkeleton*, int) asm("getEnergyCost__19SkillSummonSkeletoni");
extern "C" float get_energy_cost(const SkillSummonSkeleton*, int rank) {
    return static_cast<float>(static_cast<int>(static_cast<unsigned>(rank) - 1u)) * 5.0f + 81.0f;
}
