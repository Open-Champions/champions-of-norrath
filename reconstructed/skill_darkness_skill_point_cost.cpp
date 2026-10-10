// Independently authored from the behavior of the hash-pinned Norrath ELF.
// Original symbol: getSkillPointCost__13SkillDarknessi
// Address: 0x002982f0; complete symbol extent: 12 bytes.
// Multiplies the second argument by three with 32-bit wrapping arithmetic.
// The comparison requires the recorded compiler option -fwrapv.
// The retained name suggests skill point cost; gameplay use is not yet run.
// The receiver is unused. This C entry point is a comparison adapter.
struct SkillDarkness;

static_assert(sizeof(int) == 4, "The comparison requires a 32-bit int.");

extern "C" int norrath_darkness_skill_point_cost(const SkillDarkness*, int rank) {
    return rank * 3;
}
