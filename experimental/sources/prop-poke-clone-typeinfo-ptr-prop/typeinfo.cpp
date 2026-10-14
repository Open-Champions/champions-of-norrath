// Observed __tfP4Prop at 0x312d30, 80 bytes.
// The 12-byte typeinfo object is __tiP4Prop at 0x8eb2b8, a zero-initialized
// object. When its first word is zero, the function calls __tf4Prop and
// then __rtti_ptr with this object, the name "P4Prop" (0x483a98), and
// __ti4Prop. A nonzero first word makes neither call. It returns the
// object.

extern "C" {
unsigned char norrath_ti_P4Prop[12];
void* __tf4Prop();
void norrath_rtti_ptr(void* typeinfo, const char* name, void* target);
extern unsigned char norrath_ti_4Prop[];
}

extern "C" void* __tfP4Prop() {
    unsigned char* raw = norrath_ti_P4Prop;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        __tf4Prop();
        norrath_rtti_ptr(raw, "P4Prop", norrath_ti_4Prop);
    }
    return raw;
}
