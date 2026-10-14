// Observed __tf10HateBridge at 0x003156f8, 80 bytes.
// Returns the 12-byte __ti10HateBridge object at 0x8eb548 (COMMON in the image, so zero in the file).
// When its first word is zero, calls the AutoProp type-info function, then __rtti_si with
// this object, the bytes "10HateBridge" (at 0x471370), and the AutoProp type-info object.
// A nonzero first word skips both calls.

extern "C" {
unsigned char __ti10HateBridge[12];
void* __tf8AutoProp();
extern unsigned char __ti8AutoProp[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf10HateBridge() {
    const unsigned char* raw = __ti10HateBridge;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        __tf8AutoProp();
        __rtti_si(__ti10HateBridge, "10HateBridge", __ti8AutoProp);
    }
    return __ti10HateBridge;
}
