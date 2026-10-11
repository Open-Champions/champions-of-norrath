// Observed __tf17charAmbient_Light at 0x30d838, 76 bytes.
// The 12-byte typeinfo object is __ti17charAmbient_Light at 0x8eb258. It is COMMON,
// section type NOBITS, so its file image is zero. The first word gates
// initialization. A zero word calls __tf10GameObject, then __rtti_si with
// this object, the class name, and __ti10GameObject. gp 0x004879f0 minus
// 3112 is 0x00486dc8, the GameObject typeinfo. A nonzero first word
// returns the object and makes neither call.

extern "C" {
unsigned char norrath_ti_17charAmbient_Light[12];
void* norrath_vt_tf_10GameObject();
void norrath_rtti_si(void* typeinfo, const char* name, void* base);
extern unsigned char norrath_ti_10GameObject[];
}

extern "C" void* norrath_vt_tf_17charAmbient_Light() {
    unsigned char* raw = norrath_ti_17charAmbient_Light;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        norrath_vt_tf_10GameObject();
        norrath_rtti_si(raw, "17charAmbient_Light", norrath_ti_10GameObject);
    }
    return raw;
}
