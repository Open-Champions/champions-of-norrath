// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002be3b0 (92 bytes):
// t = table entry at byte 120 (receiver, rank, 1); return table entry at byte 88 (receiver, 1, t) as an int.
// Slot 15 names tweekRank__5Skillii, slot 11 evaluateRampAtRank__5Skillii.
struct SkillLifeTap;
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
extern "C" int get_amount(SkillLifeTap* self, int rank) asm("getAmount__12SkillLifeTapi");
extern "C" int get_amount(SkillLifeTap* self, int rank) {
    unsigned char* object = reinterpret_cast<unsigned char*>(self);
    const int tweaked = call_entry(object, 120, rank, 1);
    return call_entry(object, 88, 1, tweaked);
}
