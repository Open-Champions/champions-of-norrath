// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002a5f08 (52 bytes):
// call the energy-cost-ramp virtual method (vtable pair 16, byte offset 128 of the table whose pointer is the word at
// receiver offset 96; the pair's halfword is the receiver adjustment) with the rank and the constant 19, and return its float.
struct SkillBash;
struct VtPair { short adjustment; short pad; float (*function)(void*, int, int); };
extern "C" float get_energy_cost(SkillBash*, int) asm("getEnergyCost__9SkillBashi");
extern "C" float get_energy_cost(SkillBash* self, int rank) {
    char* base = reinterpret_cast<char*>(self);
    const VtPair* table = *reinterpret_cast<const VtPair* const*>(base + 96);
    const VtPair* pair = table + 16;
    return pair->function(base + pair->adjustment, rank, 19);
}
