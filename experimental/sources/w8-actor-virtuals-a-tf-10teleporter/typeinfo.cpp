// Observed __tf10Teleporter at 0x00312830, 80 bytes.
// Returns the 12-byte __ti10Teleporter object at 0x8eaf28 (COMMON in the image, so zero in the file).
// When its first word is zero, calls the Creature type-info function, then __rtti_si with
// this object, the bytes "10Teleporter" (at 0x4611a8), and the Creature type-info object.
// A nonzero first word skips both calls.

extern "C" {
unsigned char __ti10Teleporter[12];
void* __tf8Creature();
extern unsigned char __ti8Creature[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf10Teleporter() {
    const unsigned char* raw = __ti10Teleporter;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        __tf8Creature();
        __rtti_si(__ti10Teleporter, "10Teleporter", __ti8Creature);
    }
    return __ti10Teleporter;
}
