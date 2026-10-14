// Observed __tf8Froglock at 0x0030ef90, 80 bytes (weak definition).
// Returns the 12-byte __ti8Froglock object. When its first word is zero, calls
// the Creature type-info function, then __rtti_si with this object,
// the bytes "8Froglock", and the Creature type-info object.
// A nonzero first word skips both calls.

extern "C" {
unsigned char __ti8Froglock[12];
void* __tf8Creature();
extern unsigned char __ti8Creature[];
void __rtti_si(void* typeinfo, const char* name, void* base);
}

extern "C" void* __tf8Froglock() {
    const unsigned char* raw = __ti8Froglock;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        __tf8Creature();
        __rtti_si(__ti8Froglock, "8Froglock", __ti8Creature);
    }
    return __ti8Froglock;
}
