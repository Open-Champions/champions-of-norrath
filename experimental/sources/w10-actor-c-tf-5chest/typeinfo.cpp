// Observed __tf5Chest at 0x0030e198, 80 bytes.
// Returns the 12-byte __ti5Chest object (COMMON in the image, zero in the file).
// When its first word is zero, calls the 10GameObject type-info function, then __rtti_si with
// this object, the bytes "5Chest" (at 0x00483940) and the 10GameObject type-info object.
// A nonzero first word skips both calls.
// Inferred: the object is the class's runtime type descriptor, from the symbol names.

extern "C" {
unsigned char __ti5Chest[12];
void* __tf10GameObject();
extern unsigned char __ti10GameObject[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf5Chest() {
    const unsigned char* raw = __ti5Chest;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        __tf10GameObject();
        __rtti_si(__ti5Chest, "5Chest", __ti10GameObject);
    }
    return __ti5Chest;
}
