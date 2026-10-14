// Observed __tf5Demon at 0x00314080, 80 bytes.
// Returns the 12-byte __ti5Demon object. When its first word is zero, calls
// the Creature type-info function, then __rtti_si with this object,
// the bytes "5Demon", and the Creature type-info object.
// A nonzero first word skips both calls.

extern "C" {
unsigned char __ti5Demon[12];
void* __tf8Creature();
extern unsigned char __ti8Creature[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf5Demon() {
    const unsigned char* raw = __ti5Demon;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        __tf8Creature();
        __rtti_si(__ti5Demon, "5Demon", __ti8Creature);
    }
    return __ti5Demon;
}
