// Observed __tf10BlackWidow at 0x0030eae0, 80 bytes.
// Returns the 12-byte __ti10BlackWidow object (COMMON in the image, zero in the file).
// When its first word is zero, calls the 8Creature type-info function, then __rtti_si with
// this object, the bytes "10BlackWidow" (at 0x00460da8) and the 8Creature type-info object.
// A nonzero first word skips both calls.
// Inferred: the object is the class's runtime type descriptor, from the symbol names.

extern "C" {
unsigned char __ti10BlackWidow[12];
void* __tf8Creature();
extern unsigned char __ti8Creature[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf10BlackWidow() {
    const unsigned char* raw = __ti10BlackWidow;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        __tf8Creature();
        __rtti_si(__ti10BlackWidow, "10BlackWidow", __ti8Creature);
    }
    return __ti10BlackWidow;
}
