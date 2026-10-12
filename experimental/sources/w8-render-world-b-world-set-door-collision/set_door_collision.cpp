// Observed worldSetDoorCollision__FP12_worldHeaderii at 0x0017d348, 132 bytes.
// A negative index does nothing. Otherwise the door table at header+84 is
// indexed by 28-byte records; the signed half at record+24 selects a 36-byte
// record in the table at header+32 (as in worldGetDoorHeight, authored by
// another package). The half-word at that record+16 is updated: a nonzero
// third argument clears bit 1 (mask 0xfffd), zero sets bit 1. Nothing is
// returned. The meaning of the bit is not established here.
#if defined(NORRATH_HOST_DOOR)
extern unsigned char* norrath_door_arena;
#endif

namespace {
inline unsigned char* table_at(unsigned char* header, unsigned offset) {
    const unsigned bits = *reinterpret_cast<const unsigned*>(header + offset);
#if defined(NORRATH_HOST_DOOR)
    return norrath_door_arena + bits;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
}

extern "C" void worldSetDoorCollision__FP12_worldHeaderii(void* world, int index, int enable) {
    if (index < 0) return;
    unsigned char* header = static_cast<unsigned char*>(world);
    const unsigned char* door = table_at(header, 84u) + index * 28;
    const int slot = *reinterpret_cast<const short*>(door + 24);
    unsigned short* flags = reinterpret_cast<unsigned short*>(table_at(header, 32u) + slot * 36 + 16);
    if (enable != 0) {
        *flags = static_cast<unsigned short>(*flags & 0xfffdu);
    } else {
        *flags = static_cast<unsigned short>(*flags | 2u);
    }
}
