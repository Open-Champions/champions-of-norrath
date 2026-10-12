// Observed __tf13TriggerParams at 0x0030e498, 76 bytes.
// Returns the 12-byte __ti13TriggerParams object. When its first word is zero, calls
// the GameObject type-info function, then __rtti_si with this object,
// the bytes "13TriggerParams", and the GameObject type-info object.
// A nonzero first word skips both calls.

extern "C" {
unsigned char __ti13TriggerParams[12];
void* norrath_vt_tf_10GameObject();
extern unsigned char norrath_ti_10GameObject[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf13TriggerParams() {
    const unsigned char* raw = __ti13TriggerParams;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        norrath_vt_tf_10GameObject();
        __rtti_si(__ti13TriggerParams, "13TriggerParams", norrath_ti_10GameObject);
    }
    return __ti13TriggerParams;
}
