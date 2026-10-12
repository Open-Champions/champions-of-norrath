// Observed worldGetDoorHeight__FP12_worldHeaderi at 0x0017d3d0, 64 bytes.
// A negative index returns 0. Otherwise the door table at header+84 is
// indexed by 28-byte records. The signed half at record+24 selects a
// 36-byte record in the table at header+32. The word at that record+28
// is the returned integer. HateHole converts it with cvt.s.w.
#if defined(NORRATH_HOST_DOOR)
extern unsigned char* norrath_door_arena;
#endif

namespace {
__attribute__((always_inline)) inline unsigned load_word(const unsigned char* address) {
    return address[0] | (unsigned(address[1]) << 8) | (unsigned(address[2]) << 16) |
           (unsigned(address[3]) << 24);
}
__attribute__((always_inline)) inline int load_half(const unsigned char* address) {
    const unsigned value = address[0] | (unsigned(address[1]) << 8);
    return value < 0x8000u ? static_cast<int>(value) : static_cast<int>(value) - 65536;
}
__attribute__((always_inline)) inline const unsigned char* table_at(const unsigned char* header, unsigned offset) {
    const unsigned bits = load_word(header + offset);
#if defined(NORRATH_HOST_DOOR)
    return norrath_door_arena + bits;
#else
    return reinterpret_cast<const unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
}

extern "C" int norrath_world_get_door_height(void* world, int index) {
    if (index < 0) return 0;
    const auto* header = static_cast<const unsigned char*>(world);
    const unsigned char* door = table_at(header, 84u) + index * 28;
    const int slot = load_half(door + 24);
    const unsigned char* item = table_at(header, 32u) + slot * 36;
    return static_cast<int>(load_word(item + 28));
}
