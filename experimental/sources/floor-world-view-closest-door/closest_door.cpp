// Observed worldFindClosestDoor__FP12_worldHeaderG6Point3 at 0x00175440,
// 432 bytes. The point is the 12 bytes addressed by the second argument.
// Header+80 is a signed door count. Header+84 is the 28-byte door table.
// Header+36 is a 60-byte table selected by the word at door+0.
// Door words at +8, +12 and +16 are converted with cvt.s.w and multiplied
// by 0.25. The signed halves at +20 and +22 are converted and multiplied
// by the .lit4 bits 0x38000100 at 0x00480000. Bit 0x40 of the 60-byte
// record's word at +36 selects which pair of products is added. The
// squared distance starts against 6400 (bits 0x45c80000). A distance that
// is not strictly greater replaces the best, so a later tie wins. The
// chosen door's halfword at +6 has bit 0 set unless that bit was already
// set, in which case the result is -1.
#if defined(NORRATH_HOST_DOOR)
extern unsigned char* norrath_door_arena;
#endif

namespace {
__attribute__((always_inline)) inline unsigned load_word(const unsigned char* address) {
    return address[0] | (unsigned(address[1]) << 8) | (unsigned(address[2]) << 16) |
           (unsigned(address[3]) << 24);
}
__attribute__((always_inline)) inline int load_signed(const unsigned char* address) {
    const unsigned value = load_word(address);
    if (value < 0x80000000u) return static_cast<int>(value);
    return static_cast<int>(static_cast<long long>(value) - 0x100000000LL);
}
__attribute__((always_inline)) inline int load_half(const unsigned char* address) {
    const unsigned value = address[0] | (unsigned(address[1]) << 8);
    return value < 0x8000u ? static_cast<int>(value) : static_cast<int>(value) - 65536;
}
__attribute__((always_inline)) inline void store_half(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
}
__attribute__((always_inline)) inline float load_float(const unsigned char* address) {
    union {
        unsigned bits;
        float value;
    } converted;
    converted.bits = load_word(address);
    return converted.value;
}
__attribute__((always_inline)) inline float bits_float(unsigned bits) {
    union {
        unsigned bits;
        float value;
    } converted;
    converted.bits = bits;
    return converted.value;
}
__attribute__((always_inline)) inline unsigned char* table_at(const unsigned char* header, unsigned offset) {
    const unsigned bits = load_word(header + offset);
#if defined(NORRATH_HOST_DOOR)
    return norrath_door_arena + bits;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
}

extern "C" int norrath_world_find_closest_door(void* world, const void* point) {
    const auto* header = static_cast<const unsigned char*>(world);
    const auto* point_bytes = static_cast<const unsigned char*>(point);
    const float point_x = load_float(point_bytes);
    const float point_y = load_float(point_bytes + 4);
    const float point_z = load_float(point_bytes + 8);
    const float quarter = 0.25f;
    const float angle_scale = bits_float(0x38000100u);
    float best = bits_float(0x45c80000u);
    int chosen = -1;
    const int count = load_signed(header + 80);
    if (count > 0) {
        const unsigned char* doors = table_at(header, 84u);
        const unsigned char* elements = table_at(header, 36u);
        for (int index = 0; index < count; ++index) {
            const unsigned char* door = doors + index * 28;
            const int element_index = load_signed(door);
            const unsigned char* element = elements + element_index * 60;
            const float along = static_cast<float>(load_signed(door + 8)) * quarter;
            const float across = static_cast<float>(load_signed(door + 12)) * quarter;
            const float height = static_cast<float>(load_signed(door + 16)) * quarter;
            const float scaled_20 = static_cast<float>(load_half(door + 20)) * angle_scale;
            const float scaled_22 = static_cast<float>(load_half(door + 22)) * angle_scale;
            float x = 0.0f;
            float y = 0.0f;
            if ((load_word(element + 36) & 0x40u) != 0u) {
                x = along * scaled_20 + across * scaled_22;
                y = along * scaled_22 - across * scaled_20;
            } else {
                x = along * scaled_20 - across * scaled_22;
                y = along * scaled_22 + across * scaled_20;
            }
            const float dx = point_x - x;
            const float dy = point_y - y;
            const float dz = point_z - height;
            const float distance = (dx * dx + dy * dy) + dz * dz;
            if (!(best < distance)) {
                best = distance;
                chosen = index;
            }
        }
    }
    if (chosen < 0) return -1;
    unsigned char* door = table_at(header, 84u) + chosen * 28;
    const unsigned flags = door[6] | (unsigned(door[7]) << 8);
    if ((flags & 1u) != 0u) return -1;
    store_half(door + 6, flags | 1u);
    return chosen;
}
