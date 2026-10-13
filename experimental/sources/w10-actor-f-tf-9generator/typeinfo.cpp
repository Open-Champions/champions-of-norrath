// Observed __tf9Generator at 0x0030f050, 80 bytes (weak definition).
// Returns the 12-byte __ti9Generator object. When its first word is zero, calls
// the Creature type-info function, then __rtti_si with this object,
// the bytes "9Generator", and the Creature type-info object.
// A nonzero first word skips both calls.

extern "C" {
unsigned char __ti9Generator[12];
void* __tf8Creature();
extern unsigned char __ti8Creature[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf9Generator() {
    const unsigned char* raw = __ti9Generator;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        __tf8Creature();
        __rtti_si(__ti9Generator, "9Generator", __ti8Creature);
    }
    return __ti9Generator;
}
