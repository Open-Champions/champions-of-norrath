// Observed __tf7Counter at 0x00312498, 76 bytes.
// Returns the 12-byte __ti7Counter object. When its first word is zero, calls
// the GameObject type-info function, then __rtti_si with this object,
// the bytes "7Counter", and the GameObject type-info object.
// A nonzero first word skips both calls.

extern "C" {
unsigned char __ti7Counter[12];
void* norrath_vt_tf_10GameObject();
extern unsigned char norrath_ti_10GameObject[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf7Counter() {
    const unsigned char* raw = __ti7Counter;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        norrath_vt_tf_10GameObject();
        __rtti_si(__ti7Counter, "7Counter", norrath_ti_10GameObject);
    }
    return __ti7Counter;
}
