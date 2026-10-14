// Observed __tf7Player1 at 0x30d780, 76 bytes.
// The 12-byte typeinfo object is __ti7Player1 at 0x8eac08, a zero-initialized
// object. Its first word gates initialization. A zero word calls the
// GameObject typeinfo function, then __rtti_si with this object, the class
// name "7Player1", and the GameObject typeinfo object (gp 0x4879f0 minus 3112
// is 0x486dc8). A nonzero first word returns the object and makes neither
// call. The authored name of the object is norrath_ti_7Player1.

extern "C" {
unsigned char norrath_ti_7Player1[12];
void* norrath_vt_tf_10GameObject();
void norrath_rtti_si(void* typeinfo, const char* name, void* base);
extern unsigned char norrath_ti_10GameObject[];
}

extern "C" void* __tf7Player1() {
    unsigned char* raw = norrath_ti_7Player1;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        norrath_vt_tf_10GameObject();
        norrath_rtti_si(raw, "7Player1", norrath_ti_10GameObject);
    }
    return raw;
}
