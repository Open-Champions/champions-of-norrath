// Observed __tf8FireBomb at 0x00312168, 76 bytes.
// Returns the 12-byte __ti8FireBomb object at 0x8eaea8 (COMMON in the image, so zero in the file).
// When its first word is zero, calls the GameObject type-info function, then __rtti_si with
// this object, the bytes "8FireBomb" (at 0x461128), and the GameObject type-info object.
// A nonzero first word skips both calls.

extern "C" {
unsigned char __ti8FireBomb[12];
void* norrath_vt_tf_10GameObject();
extern unsigned char norrath_ti_10GameObject[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf8FireBomb() {
    const unsigned char* raw = __ti8FireBomb;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        norrath_vt_tf_10GameObject();
        __rtti_si(__ti8FireBomb, "8FireBomb", norrath_ti_10GameObject);
    }
    return __ti8FireBomb;
}
