// Observed __tf8Catapult at 0x003169d8, 80 bytes.
// Returns the 12-byte __ti8Catapult object (COMMON in the image, zero in the file).
// When its first word is zero, calls the 10GameObject type-info function, then __rtti_si with
// this object, the bytes "8Catapult" (at 0x00477838) and the 10GameObject type-info object.
// A nonzero first word skips both calls.
// Inferred: the object is the class's runtime type descriptor, from the symbol names.

extern "C" {
unsigned char __ti8Catapult[12];
void* __tf10GameObject();
extern unsigned char __ti10GameObject[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf8Catapult() {
    const unsigned char* raw = __ti8Catapult;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        __tf10GameObject();
        __rtti_si(__ti8Catapult, "8Catapult", __ti10GameObject);
    }
    return __ti8Catapult;
}
