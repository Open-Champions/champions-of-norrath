// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002bbd30 (36 bytes):
// return (rank - 1) * 0x1.40001cp+2f + 107.0 as a single float: the rank is decremented with wraparound, converted to
// float, multiplied by the per-rank step and added to the rank-one cost. The receiver is not read.
struct SkillDismissUndead;
extern "C" float get_energy_cost(const SkillDismissUndead*, int) asm("getEnergyCost__18SkillDismissUndeadi");
extern "C" float get_energy_cost(const SkillDismissUndead*, int rank) {
    return static_cast<float>(static_cast<int>(static_cast<unsigned>(rank) - 1u)) * 0x1.40001cp+2f + 107.0f;
}
