// Observed __tf5Lever at 0x003123e0, 76 bytes.
// Returns the 12-byte __ti5Lever object. When its first word is zero, calls
// the GameObject type-info function, then __rtti_si with this object,
// the bytes "5Lever", and the GameObject type-info object.
// A nonzero first word skips both calls.

extern "C" {
unsigned char __ti5Lever[12];
void* norrath_vt_tf_10GameObject();
extern unsigned char norrath_ti_10GameObject[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf5Lever() {
    const unsigned char* raw = __ti5Lever;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        norrath_vt_tf_10GameObject();
        __rtti_si(__ti5Lever, "5Lever", norrath_ti_10GameObject);
    }
    return __ti5Lever;
}
