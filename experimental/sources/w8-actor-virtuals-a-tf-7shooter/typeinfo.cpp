// Observed __tf7Shooter at 0x003108f8, 76 bytes.
// Returns the 12-byte __ti7Shooter object at 0x8eac78 (COMMON in the image, so zero in the file).
// When its first word is zero, calls the GameObject type-info function, then __rtti_si with
// this object, the bytes "7Shooter" (at 0x460f70), and the GameObject type-info object.
// A nonzero first word skips both calls.

extern "C" {
unsigned char __ti7Shooter[12];
void* norrath_vt_tf_10GameObject();
extern unsigned char norrath_ti_10GameObject[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf7Shooter() {
    const unsigned char* raw = __ti7Shooter;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        norrath_vt_tf_10GameObject();
        __rtti_si(__ti7Shooter, "7Shooter", norrath_ti_10GameObject);
    }
    return __ti7Shooter;
}
