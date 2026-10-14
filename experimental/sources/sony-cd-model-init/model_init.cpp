// Observed modelInitModel__FPv at 0x0016c550, 108 bytes.
// A nonpositive signed byte at +38 is replaced with 1. The word count is that
// byte times one more than the signed byte at +18. That many words at +40 are
// relocated by adding the model address.
extern "C" {
static int load_byte(const unsigned char* slot) {
    const int value = slot[0];
    return value < 128 ? value : value - 256;
}
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
void modelInitModel__FPv(void* model) {
    auto* bytes = static_cast<unsigned char*>(model);
    if (load_byte(bytes + 38) <= 0) bytes[38] = 1;
    const int count = load_byte(bytes + 38) * (load_byte(bytes + 18) + 1);
    const unsigned base = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(model));
    for (int index = 0; index < count; ++index) {
        unsigned char* slot = bytes + 40 + index * 4;
        store_word(slot, load_word(slot) + base);
    }
}
}
