// Observed __tf13UserParamProp at 0x30e3e0, 76 bytes.
// The 12-byte typeinfo object is __ti13UserParamProp at 0x8eb188, a zero-initialized
// object. Its first word gates initialization. A zero word calls the
// GameObject typeinfo function, then __rtti_si with this object, the class
// name "13UserParamProp", and the GameObject typeinfo object (gp 0x4879f0 minus 3112
// is 0x486dc8). A nonzero first word returns the object and makes neither
// call. The authored name of the object is norrath_ti_13UserParamProp.

extern "C" {
unsigned char norrath_ti_13UserParamProp[12];
void* norrath_vt_tf_10GameObject();
void norrath_rtti_si(void* typeinfo, const char* name, void* base);
extern unsigned char norrath_ti_10GameObject[];
}

extern "C" void* __tf13UserParamProp() {
    unsigned char* raw = norrath_ti_13UserParamProp;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        norrath_vt_tf_10GameObject();
        norrath_rtti_si(raw, "13UserParamProp", norrath_ti_10GameObject);
    }
    return raw;
}
