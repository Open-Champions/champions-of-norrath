// Observed worldSetDoorCollision__FP12_worldHeaderii at 0x0017d348, 132 bytes.
// A negative index returns without a store. The same 28-byte door records
// and 36-byte records used by worldGetDoorHeight select a halfword at
// item+16. A zero third argument sets bit 1. Any other value clears bit 1.
// The other bits of that halfword stay as they were. The original V0 on the
// write path is the item address; the constructors ignore it.
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
__attribute__((always_inline)) inline unsigned load_uhalf(const unsigned char* address) {
    return address[0] | (unsigned(address[1]) << 8);
}
__attribute__((always_inline)) inline void store_half(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
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

extern "C" int norrath_world_set_door_collision(void* world, int index, int enabled) {
    if (index < 0) return 0;
    const auto* header = static_cast<const unsigned char*>(world);
    const unsigned char* door = table_at(header, 84u) + index * 28;
    const int slot = load_half(door + 24);
    unsigned char* item = const_cast<unsigned char*>(table_at(header, 32u) + slot * 36);
    unsigned flags = load_uhalf(item + 16);
    flags = enabled == 0 ? (flags | 2u) : (flags & 0xfffdu);
    store_half(item + 16, flags);
    return 0;
}
