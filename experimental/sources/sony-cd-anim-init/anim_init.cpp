// Observed animInitAnimation__FP15AnimationHeader at 0x001637b0, 124 bytes.
// Words at +8, +12, and +16 are relocated by the header address. A nonzero
// word at +52 becomes a pointer, and each nonzero word at stride 8 from there
// is relocated until a zero word. A nonzero word at +56 is relocated as well.
extern "C" {
static unsigned load_word(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) | (static_cast<unsigned>(slot[3]) << 24);
}
static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
static unsigned pointer_bits(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
static void relocate(unsigned char* header, unsigned offset) {
    store_word(header + offset, pointer_bits(header + load_word(header + offset)));
}
void animInitAnimation__FP15AnimationHeader(void* header_pointer) {
    auto* header = static_cast<unsigned char*>(header_pointer);
    relocate(header, 8u);
    relocate(header, 16u);
    relocate(header, 12u);
    const unsigned list = load_word(header + 52);
    if (list != 0u) {
        auto* cursor = header + list;
        store_word(header + 52, pointer_bits(cursor));
        if (load_word(cursor) != 0u) {
            do {
                store_word(cursor, pointer_bits(header + load_word(cursor)));
                cursor += 8;
            } while (load_word(cursor) != 0u);
        }
    }
    if (load_word(header + 56) != 0u) relocate(header, 56u);
}
}
