// Observed __tf22charDirectional_LightD at 0x30d9a8, 76 bytes.
// The 12-byte typeinfo object is __ti22charDirectional_LightD at 0x8eb1a8. It is COMMON,
// section type NOBITS, so its file image is zero. The first word gates
// initialization. A zero word calls __tf10GameObject, then __rtti_si with
// this object, the class name, and __ti10GameObject. gp 0x004879f0 minus
// 3112 is 0x00486dc8, the GameObject typeinfo. A nonzero first word
// returns the object and makes neither call.

extern "C" {
unsigned char norrath_ti_22charDirectional_LightD[12];
void* norrath_vt_tf_10GameObject();
void norrath_rtti_si(void* typeinfo, const char* name, void* base);
extern unsigned char norrath_ti_10GameObject[];
}

extern "C" void* norrath_vt_tf_22charDirectional_LightD() {
    unsigned char* raw = norrath_ti_22charDirectional_LightD;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        norrath_vt_tf_10GameObject();
        norrath_rtti_si(raw, "22charDirectional_LightD", norrath_ti_10GameObject);
    }
    return raw;
}
