// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002a7010 (52 bytes):
// call the Skill table entry at byte 128 with (receiver, rank, 1) and return its float result untouched.
// In every Skill table of the image slot 16 (byte 128) names energyCostRamp4__5Skillii; what 1 means is unknown.
struct SkillBlessed;
#ifdef W13_HOST
extern "C" const unsigned char* w13_host_table(const unsigned char* self);
extern "C" float w13_host_call(unsigned function, unsigned char* adjusted, int a, int b);
#endif
// Calls the table entry at the given byte offset of the table whose address is the word at self+96: the signed halfword at the
// entry adjusts the receiver, the word at entry+4 is the function; it takes (receiver, a, b) and returns float.
static inline float call_entry(unsigned char* self, int offset, int a, int b) {
#ifdef W13_HOST
    const unsigned char* entry = w13_host_table(self) + offset;
#else
    const unsigned char* entry = reinterpret_cast<const unsigned char*>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<const unsigned*>(self + 96))) + offset;
#endif
    unsigned char* adjusted = self + *reinterpret_cast<const short*>(entry);
    const unsigned function = *reinterpret_cast<const unsigned*>(entry + 4);
#ifdef W13_HOST
    return w13_host_call(function, adjusted, a, b);
#else
    return reinterpret_cast<float (*)(unsigned char*, int, int)>(static_cast<__UINTPTR_TYPE__>(function))(adjusted, a, b);
#endif
}
extern "C" float get_energy_cost(SkillBlessed* self, int rank) asm("getEnergyCost__12SkillBlessedi");
extern "C" float get_energy_cost(SkillBlessed* self, int rank) {
    return call_entry(reinterpret_cast<unsigned char*>(self), 128, rank, 1);
}
