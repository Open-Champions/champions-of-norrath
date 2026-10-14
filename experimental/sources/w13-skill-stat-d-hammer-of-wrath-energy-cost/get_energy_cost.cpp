// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002b5818 (40 bytes):
// return (rank - 1) * 4.0f + 48.0 as a single float: the rank is decremented with wraparound, converted to
// float, multiplied by the per-rank step and added to the rank-one cost. The receiver is not read.
struct SkillHammerOfWrath;
extern "C" float get_energy_cost(const SkillHammerOfWrath*, int) asm("getEnergyCost__18SkillHammerOfWrathi");
extern "C" float get_energy_cost(const SkillHammerOfWrath*, int rank) {
    return static_cast<float>(static_cast<int>(static_cast<unsigned>(rank) - 1u)) * 4.0f + 48.0f;
}
