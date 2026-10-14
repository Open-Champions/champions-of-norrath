// Observed __tf9WorldPart at 0x0030d9f8, 76 bytes.
// Returns the 12-byte __ti9WorldPart object. When its first word is zero, calls
// the GameObject type-info function, then __rtti_si with this object,
// the bytes "9WorldPart", and the GameObject type-info object.
// A nonzero first word skips both calls.

extern "C" {
unsigned char __ti9WorldPart[12];
void* norrath_vt_tf_10GameObject();
extern unsigned char norrath_ti_10GameObject[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf9WorldPart() {
    const unsigned char* raw = __ti9WorldPart;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        norrath_vt_tf_10GameObject();
        __rtti_si(__ti9WorldPart, "9WorldPart", norrath_ti_10GameObject);
    }
    return __ti9WorldPart;
}
