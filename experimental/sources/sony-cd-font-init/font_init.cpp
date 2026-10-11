// Observed fontInitFont__FP4Font at 0x00174350, 64 bytes.
// Words at +8, +12, and +16 are relocated by the font address. The relocated
// word at +16 is passed to textureInitTexture.
extern "C" {
void textureInitTexture__FP8_texture(void* texture);
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
void fontInitFont__FP4Font(void* font_pointer) {
    auto* font = static_cast<unsigned char*>(font_pointer);
    auto* at_8 = font + load_word(font + 8);
    auto* at_12 = font + load_word(font + 12);
    auto* at_16 = font + load_word(font + 16);
    store_word(font + 8, pointer_bits(at_8));
    store_word(font + 12, pointer_bits(at_12));
    store_word(font + 16, pointer_bits(at_16));
    textureInitTexture__FP8_texture(at_16);
}
}
