// Observed __tf8GothTrap at 0x00315588, 80 bytes.
// Returns the 12-byte __ti8GothTrap object at 0x8eb598 (COMMON in the image, so zero in the file).
// When its first word is zero, calls the AutoProp type-info function, then __rtti_si with
// this object, the bytes "8GothTrap" (at 0x471350), and the AutoProp type-info object.
// A nonzero first word skips both calls.

extern "C" {
unsigned char __ti8GothTrap[12];
void* __tf8AutoProp();
extern unsigned char __ti8AutoProp[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf8GothTrap() {
    const unsigned char* raw = __ti8GothTrap;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        __tf8AutoProp();
        __rtti_si(__ti8GothTrap, "8GothTrap", __ti8AutoProp);
    }
    return __ti8GothTrap;
}
