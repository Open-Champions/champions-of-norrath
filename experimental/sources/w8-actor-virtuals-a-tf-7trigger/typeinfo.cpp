// Observed __tf7Trigger at 0x0030e550, 76 bytes.
// Returns the 12-byte __ti7Trigger object at 0x8eacd8 (COMMON in the image, so zero in the file).
// When its first word is zero, calls the GameObject type-info function, then __rtti_si with
// this object, the bytes "7Trigger" (at 0x460d58), and the GameObject type-info object.
// A nonzero first word skips both calls.

extern "C" {
unsigned char __ti7Trigger[12];
void* norrath_vt_tf_10GameObject();
extern unsigned char norrath_ti_10GameObject[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf7Trigger() {
    const unsigned char* raw = __ti7Trigger;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        norrath_vt_tf_10GameObject();
        __rtti_si(__ti7Trigger, "7Trigger", norrath_ti_10GameObject);
    }
    return __ti7Trigger;
}
