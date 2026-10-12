// Observed __tf3NPC at 0x0030fb50, 80 bytes.
// Returns the 12-byte __ti3NPC object at 0x8eb058 (COMMON in the image, so zero in the file).
// When its first word is zero, calls the Creature type-info function, then __rtti_si with
// this object, the bytes "3NPC" (at 0x483998), and the Creature type-info object.
// A nonzero first word skips both calls.

extern "C" {
unsigned char __ti3NPC[12];
void* __tf8Creature();
extern unsigned char __ti8Creature[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf3NPC() {
    const unsigned char* raw = __ti3NPC;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        __tf8Creature();
        __rtti_si(__ti3NPC, "3NPC", __ti8Creature);
    }
    return __ti3NPC;
}
