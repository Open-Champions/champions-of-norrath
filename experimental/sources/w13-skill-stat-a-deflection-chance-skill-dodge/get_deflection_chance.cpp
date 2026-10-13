// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002ae7d8 (116 bytes):
// if rank is zero return 0.0f. Otherwise t = table entry at byte 120 (receiver, rank, 1), then
// r = table entry at byte 88 (receiver, 0, t); return (float)r * 0.01f (the single with bits 0x3c23d70a, a 4-byte
// small-data constant of unknown owner). Slot 15 names tweekRank__5Skillii, slot 11 evaluateRampAtRank__5Skillii.
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
extern "C" float get_deflection_chance(SkillDodge* self, int rank) asm("getDeflectionChance__10SkillDodgei");
extern "C" float get_deflection_chance(SkillDodge* self, int rank) {
    if (rank == 0) return 0.0f;
    unsigned char* object = reinterpret_cast<unsigned char*>(self);
    const int tweaked = call_entry(object, 120, rank, 1);
    const int value = call_entry(object, 88, 0, tweaked);
    return static_cast<float>(value) * __builtin_bit_cast(float, 0x3c23d70au);
}
