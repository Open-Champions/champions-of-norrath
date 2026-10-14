// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002ba740 (40 bytes):
// return (rank - 1) * 4.0f + 59.0 as a single float: the rank is decremented with wraparound, converted to
// float, multiplied by the per-rank step and added to the rank-one cost. The receiver is not read.
struct SkillRepulseUndead;
extern "C" float get_energy_cost(const SkillRepulseUndead*, int) asm("getEnergyCost__18SkillRepulseUndeadi");
extern "C" float get_energy_cost(const SkillRepulseUndead*, int rank) {
    return static_cast<float>(static_cast<int>(static_cast<unsigned>(rank) - 1u)) * 4.0f + 59.0f;
}
