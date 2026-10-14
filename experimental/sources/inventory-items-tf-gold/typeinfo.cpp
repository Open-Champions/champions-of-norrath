// Observed __tf4Gold at 0x30df60, 76 bytes.
// The 12-byte typeinfo object is __ti4Gold at 0x8eaeb8. It is COMMON, section
// type NOBITS, so its file image is zero. The first word gates initialization.
// A zero word calls __tf10GameObject, then __rtti_si with this object, the
// class name "4Gold" (0x483930), and __ti10GameObject (0x486dc8). A nonzero
// first word returns the object and makes neither call.

extern "C" {
unsigned char norrath_ti_4Gold[12];
void* norrath_vt_tf_10GameObject();
void norrath_rtti_si(void* typeinfo, const char* name, void* base);
extern unsigned char norrath_ti_10GameObject[];
}

extern "C" void* __tf4Gold() {
    unsigned char* raw = norrath_ti_4Gold;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        norrath_vt_tf_10GameObject();
        norrath_rtti_si(raw, "4Gold", norrath_ti_10GameObject);
    }
    return raw;
}
