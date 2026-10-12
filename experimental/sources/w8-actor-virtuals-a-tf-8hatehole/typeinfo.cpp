// Observed __tf8HateHole at 0x00315640, 80 bytes.
// Returns the 12-byte __ti8HateHole object at 0x8eb578 (COMMON in the image, so zero in the file).
// When its first word is zero, calls the AutoProp type-info function, then __rtti_si with
// this object, the bytes "8HateHole" (at 0x471360), and the AutoProp type-info object.
// A nonzero first word skips both calls.

extern "C" {
unsigned char __ti8HateHole[12];
void* __tf8AutoProp();
extern unsigned char __ti8AutoProp[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf8HateHole() {
    const unsigned char* raw = __ti8HateHole;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        __tf8AutoProp();
        __rtti_si(__ti8HateHole, "8HateHole", __ti8AutoProp);
    }
    return __ti8HateHole;
}
