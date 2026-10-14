// Independently authored from the behavior of the hash-pinned Norrath ELF.
// Original symbol: getLevelRequirement__17SkillResistPoisoni
// Address: 0x00313d18; complete symbol extent: 8 bytes.
// The input's low 32 bits are shifted left by two; upper bits are discarded.
// An unsigned comparison interface preserves wrapping for every input bit pattern.
// The retained member signature names a signed integer argument; its caller range
// and original return type are unverified. The receiver is not read.
// This C entry point is a comparison adapter, not the original C++ class ABI.
struct SkillResistPoison;

static_assert(sizeof(unsigned int) == 4, "The comparison requires 32-bit words.");

extern "C" unsigned int norrath_resist_poison_level_requirement(
    const SkillResistPoison*, unsigned int rank_bits) {
    return rank_bits << 2;
}
