// Observed textureInitTexture__FP8_texture at 0x0016d160, 276 bytes.
// A nonzero word at +28 returns immediately. Otherwise that word becomes -1,
// the halfword at +10 becomes -22533, and the word at +16 is relocated. Bit 0
// of the halfword at +8 stops there. Bit 0x100 clear also stops there. Otherwise
// the first word of the relocated block is relocated, and each following
// four-byte record advances a running halfword stored at +6 until a -1 byte.
extern "C" {
static int load_byte(const unsigned char* slot) {
    const int value = slot[0];
    return value < 128 ? value : value - 256;
}
static unsigned load_word(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) | (static_cast<unsigned>(slot[3]) << 24);
}
static int load_half(const unsigned char* slot) {
    const unsigned value = static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8);
    return value < 0x8000u ? static_cast<int>(value) : static_cast<int>(value) - 65536;
}
static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
static void store_half(unsigned char* slot, int value) {
    const unsigned bits = static_cast<unsigned>(value);
    slot[0] = static_cast<unsigned char>(bits);
    slot[1] = static_cast<unsigned char>(bits >> 8);
}
static unsigned pointer_bits(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
void textureInitTexture__FP8_texture(void* texture_pointer) {
    auto* texture = static_cast<unsigned char*>(texture_pointer);
    if (load_word(texture + 28) != 0u) return;
    store_half(texture + 10, -22533);
    store_word(texture + 28, 0xffffffffu);
    auto* block = texture + load_word(texture + 16);
    store_word(texture + 16, pointer_bits(block));
    const int flags = load_half(texture + 8);
    if ((flags & 1) != 0 || (flags & 0x100) == 0) return;
    store_word(block, pointer_bits(texture + load_word(block)));
    unsigned char* cursor = block + 4;
    int running = 72;
    while (load_byte(cursor) != -1) {
        const int first = load_byte(cursor);
        const int second = load_byte(cursor + 1);
        const int third = load_byte(cursor + 2);
        const int fourth = load_byte(cursor + 3);
        cursor += 4;
        int product = (((fourth - second) << 11) + 2048) * ((third - first) + 1);
        if (product < 0) product += 127;
        running += (product >> 7) + 6;
        if (fourth >= second) {
            for (int row = second; row <= fourth; ++row) {
                if (third < first) continue;
                for (int column = first; column <= third; ++column) {
                    store_word(cursor, pointer_bits(texture + load_word(cursor)));
                    cursor += 4;
                    (void)column;
                }
                (void)row;
            }
        }
    }
    store_half(texture + 6, running);
}
}
