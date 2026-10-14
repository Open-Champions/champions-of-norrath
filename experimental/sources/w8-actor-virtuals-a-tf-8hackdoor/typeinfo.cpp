// Observed __tf8HackDoor at 0x003157b0, 80 bytes.
// Returns the 12-byte __ti8HackDoor object at 0x8eb568 (COMMON in the image, so zero in the file).
// When its first word is zero, calls the AutoProp type-info function, then __rtti_si with
// this object, the bytes "8HackDoor" (at 0x471380), and the AutoProp type-info object.
// A nonzero first word skips both calls.

extern "C" {
unsigned char __ti8HackDoor[12];
void* __tf8AutoProp();
extern unsigned char __ti8AutoProp[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf8HackDoor() {
    const unsigned char* raw = __ti8HackDoor;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        __tf8AutoProp();
        __rtti_si(__ti8HackDoor, "8HackDoor", __ti8AutoProp);
    }
    return __ti8HackDoor;
}
