// Observed worldSetDoorHeight__FP12_worldHeaderif at 0x0017d410, 64 bytes.
// A negative index does nothing. Otherwise the door table at header+84 is
// indexed by 28-byte records; the signed half at record+24 selects a 36-byte
// record in the table at header+32. The float argument is converted to a
// 32-bit integer (cvt.w.s, truncating on this CPU) and stored in the word at
// that record+28. Nothing is returned.
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

extern "C" void worldSetDoorHeight__FP12_worldHeaderif(void* world, int index, float height) {
    if (index < 0) return;
    unsigned char* header = static_cast<unsigned char*>(world);
    const unsigned char* door = table_at(header, 84u) + index * 28;
    const int slot = *reinterpret_cast<const short*>(door + 24);
    *reinterpret_cast<int*>(table_at(header, 32u) + slot * 36 + 28) = static_cast<int>(height);
}
