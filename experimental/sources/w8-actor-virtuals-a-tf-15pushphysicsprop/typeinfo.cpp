// Observed __tf15PushPhysicsProp at 0x00312bd8, 76 bytes.
// Returns the 12-byte __ti15PushPhysicsProp object at 0x8eafb8 (COMMON in the image, so zero in the file).
// When its first word is zero, calls the GameObject type-info function, then __rtti_si with
// this object, the bytes "15PushPhysicsProp" (at 0x461220), and the GameObject type-info object.
// A nonzero first word skips both calls.

extern "C" {
unsigned char __ti15PushPhysicsProp[12];
void* norrath_vt_tf_10GameObject();
extern unsigned char norrath_ti_10GameObject[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf15PushPhysicsProp() {
    const unsigned char* raw = __ti15PushPhysicsProp;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        norrath_vt_tf_10GameObject();
        __rtti_si(__ti15PushPhysicsProp, "15PushPhysicsProp", norrath_ti_10GameObject);
    }
    return __ti15PushPhysicsProp;
}
