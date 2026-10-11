// Observed __tf7Natasla at 0x00315aa0, 80 bytes.
// Returns the 12-byte __ti7Natasla object. When its first word is zero, calls
// the Creature type-info function, then __rtti_si with this object,
// the bytes "7Natasla", and the Creature type-info object.
// A nonzero first word skips both calls.

extern "C" {
unsigned char __ti7Natasla[12];
void* __tf8Creature();
extern unsigned char __ti8Creature[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf7Natasla() {
    const unsigned char* raw = __ti7Natasla;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        __tf8Creature();
        __rtti_si(__ti7Natasla, "7Natasla", __ti8Creature);
    }
    return __ti7Natasla;
}
