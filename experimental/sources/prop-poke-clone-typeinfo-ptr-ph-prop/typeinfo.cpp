// Observed __tfP6PhProp at 0x312ce0, 80 bytes.
// The 12-byte typeinfo object is __tiP6PhProp at 0x8eb298, a zero-initialized
// object. When its first word is zero, the function calls __tf6PhProp and
// then __rtti_ptr with this object, the name "P6PhProp" (0x461db0), and
// __ti6PhProp. A nonzero first word makes neither call. It returns the
// object.

extern "C" {
unsigned char norrath_ti_P6PhProp[12];
void* __tf6PhProp();
void norrath_rtti_ptr(void* typeinfo, const char* name, void* target);
extern unsigned char norrath_ti_6PhProp[];
}

extern "C" void* __tfP6PhProp() {
    unsigned char* raw = norrath_ti_P6PhProp;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        __tf6PhProp();
        norrath_rtti_ptr(raw, "P6PhProp", norrath_ti_6PhProp);
    }
    return raw;
}
