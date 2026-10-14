// Observed __tf7Mermaid at 0x00314520, 80 bytes.
// Returns the 12-byte __ti7Mermaid object. When its first word is zero, calls
// the Creature type-info function, then __rtti_si with this object,
// the bytes "7Mermaid", and the Creature type-info object.
// A nonzero first word skips both calls.

extern "C" {
unsigned char __ti7Mermaid[12];
void* __tf8Creature();
extern unsigned char __ti8Creature[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf7Mermaid() {
    const unsigned char* raw = __ti7Mermaid;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        __tf8Creature();
        __rtti_si(__ti7Mermaid, "7Mermaid", __ti8Creature);
    }
    return __ti7Mermaid;
}
