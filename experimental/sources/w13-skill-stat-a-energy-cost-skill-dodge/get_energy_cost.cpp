// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002ae7a0 (56 bytes):
// call the Skill table entry at byte 120 with (receiver, rank, 1), discard its result and return 0.0f.
// In every Skill table of the image slot 15 (byte 120) names tweekRank__5Skillii. The call is made even though
// its result is unused; the return value is zero for every rank.
struct SkillDodge;
#ifdef W13_HOST
extern "C" const unsigned char* w13_host_table(const unsigned char* self);
extern "C" int w13_host_call(unsigned function, unsigned char* adjusted, int a, int b);
#endif
// Calls the table entry at the given byte offset of the table whose address is the word at self+96: the signed halfword at the
// entry adjusts the receiver, the word at entry+4 is the function; it takes (receiver, a, b) and returns int.
static inline int call_entry(unsigned char* self, int offset, int a, int b) {
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
    return reinterpret_cast<int (*)(unsigned char*, int, int)>(static_cast<__UINTPTR_TYPE__>(function))(adjusted, a, b);
#endif
}
extern "C" float get_energy_cost(SkillDodge* self, int rank) asm("getEnergyCost__10SkillDodgei");
extern "C" float get_energy_cost(SkillDodge* self, int rank) {
    call_entry(reinterpret_cast<unsigned char*>(self), 120, rank, 1);
    return 0.0f;
}
