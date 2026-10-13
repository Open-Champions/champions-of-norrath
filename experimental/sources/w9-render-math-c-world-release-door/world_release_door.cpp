// Observed worldReleaseDoor__FP12_worldHeaderi at 0x0017d318, 44 bytes. A negative
// index does nothing. Otherwise the pointer at header offset 84 addresses a table
// of 28-byte door records and the halfword at record offset 6 loses its low bit
// (and with 0xfffe). Nothing is returned.
// Inferred: bit 0 of that halfword is a "held"/"locked" style flag. Unknown: its name.
#if defined(NORRATH_HOST_DOOR)
extern unsigned char* norrath_door_arena;
#endif

namespace {
inline unsigned char* door_table(unsigned char* header) {
    const unsigned bits = *reinterpret_cast<unsigned*>(header + 84);
#if defined(NORRATH_HOST_DOOR)
    return norrath_door_arena + bits;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
}

extern "C" void worldReleaseDoor__FP12_worldHeaderi(void* world, int index) {
    if (index < 0) return;
    unsigned char* door = door_table(static_cast<unsigned char*>(world)) + index * 28;
    unsigned short* flags = reinterpret_cast<unsigned short*>(door + 6);
    *flags = static_cast<unsigned short>(*flags & 0xfffe);
}
