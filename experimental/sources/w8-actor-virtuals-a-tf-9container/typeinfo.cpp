// Observed __tf9Container at 0x0030e018, 76 bytes.
// Returns the 12-byte __ti9Container object at 0x8eab88 (COMMON in the image, so zero in the file).
// When its first word is zero, calls the GameObject type-info function, then __rtti_si with
// this object, the bytes "9Container" (at 0x460d00), and the GameObject type-info object.
// A nonzero first word skips both calls.

extern "C" {
unsigned char __ti9Container[12];
void* norrath_vt_tf_10GameObject();
extern unsigned char norrath_ti_10GameObject[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf9Container() {
    const unsigned char* raw = __ti9Container;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        norrath_vt_tf_10GameObject();
        __rtti_si(__ti9Container, "9Container", norrath_ti_10GameObject);
    }
    return __ti9Container;
}
