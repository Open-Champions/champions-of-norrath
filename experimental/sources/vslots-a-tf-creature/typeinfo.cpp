// Observed __tf8Creature at 0x0030d510, 76 bytes.
// Returns the 12-byte __ti8Creature object. When its first word is zero, calls
// the GameObject type-info function, then __rtti_si with this object, the bytes
// "8Creature", and the GameObject type-info object. A nonzero first word skips
// both calls. The observed GameObject object is __ti10GameObject.

extern "C" {
unsigned char __ti8Creature[12];
void* norrath_vt_tf_10GameObject();
void __rtti_si(void* typeinfo, const char* name, void* base);
extern unsigned char norrath_ti_10GameObject[];
}

extern "C" void* __tf8Creature() {
    const unsigned char* raw = __ti8Creature;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        norrath_vt_tf_10GameObject();
        __rtti_si(__ti8Creature, "8Creature", norrath_ti_10GameObject);
    }
    return __ti8Creature;
}
