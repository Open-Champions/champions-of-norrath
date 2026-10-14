// Observed textAddText__FPv at 0x001670c8, 300 bytes.
// Each nonzero byte starts a C string. Its bytes are mixed and masked to 13
// bits, then stored in the first empty word of table at 0x006d8fa0. The cursor
// then passes the NUL, aligns down to two bytes, and skips halfwords through
// the first zero halfword. A zero byte ends the walk. strlen is the published
// name of that call.
extern "C" {
unsigned table[0x2000];
unsigned strlen(const char* text);
static int load_byte(const unsigned char* slot) {
    const int value = slot[0];
    return value < 128 ? value : value - 256;
}
static int load_half(const unsigned char* slot) {
    const unsigned value = static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8);
    return value == 0 ? 0 : 1;
}
static unsigned arithmetic_shift(unsigned value, unsigned bits) {
    const unsigned shifted = value >> bits;
    if ((value & 0x80000000u) == 0u) return shifted;
    return shifted | (0xffffffffu << (32u - bits));
}
static unsigned pointer_bits(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
static unsigned mix(const unsigned char* text, int length) {
    unsigned hash = 0;
    for (int index = 0; index < length; ++index) {
        hash += static_cast<unsigned>(load_byte(text + index));
        hash += hash << 10;
        hash ^= arithmetic_shift(hash, 6u);
    }
    hash += hash << 3;
    hash ^= arithmetic_shift(hash, 11u);
    return hash & 0x1fffu;
}
void textAddText__FPv(void* payload) {
    auto* cursor = static_cast<unsigned char*>(payload);
    while (load_byte(cursor) != 0) {
        const int length = static_cast<int>(strlen(reinterpret_cast<char*>(cursor)));
        unsigned index = mix(cursor, length);
        while (table[index] != 0u) index = (index + 1u) & 0x1fffu;
        table[index] = pointer_bits(cursor);
        auto* follow = cursor + static_cast<unsigned>(length) + 2u;
        follow = reinterpret_cast<unsigned char*>(reinterpret_cast<__UINTPTR_TYPE__>(follow) & ~static_cast<__UINTPTR_TYPE__>(1));
        if (load_half(follow) == 0) cursor = follow + 2;
        else {
            follow += 2;
            while (load_half(follow) != 0) follow += 2;
            cursor = follow + 2;
        }
    }
}
}
