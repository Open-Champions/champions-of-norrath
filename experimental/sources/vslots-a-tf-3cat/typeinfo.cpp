// Observed __tf3Cat at 0x0030eb98, 80 bytes.
// Returns the 12-byte __ti3Cat object. When its first word is zero, calls
// the Creature type-info function, then __rtti_si with this object,
// the bytes "3Cat", and the Creature type-info object.
// A nonzero first word skips both calls.

extern "C" {
unsigned char __ti3Cat[12];
void* __tf8Creature();
extern unsigned char __ti8Creature[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf3Cat() {
    const unsigned char* raw = __ti3Cat;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        __tf8Creature();
        __rtti_si(__ti3Cat, "3Cat", __ti8Creature);
    }
    return __ti3Cat;
}
