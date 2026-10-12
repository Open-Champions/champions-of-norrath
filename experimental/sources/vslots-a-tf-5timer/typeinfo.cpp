// Observed __tf5Timer at 0x00312550, 76 bytes.
// Returns the 12-byte __ti5Timer object. When its first word is zero, calls
// the GameObject type-info function, then __rtti_si with this object,
// the bytes "5Timer", and the GameObject type-info object.
// A nonzero first word skips both calls.

extern "C" {
unsigned char __ti5Timer[12];
void* norrath_vt_tf_10GameObject();
extern unsigned char norrath_ti_10GameObject[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf5Timer() {
    const unsigned char* raw = __ti5Timer;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        norrath_vt_tf_10GameObject();
        __rtti_si(__ti5Timer, "5Timer", norrath_ti_10GameObject);
    }
    return __ti5Timer;
}
