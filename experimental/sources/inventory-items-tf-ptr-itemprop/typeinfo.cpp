// Observed __tfP8ItemProp at 0x312d80, 80 bytes.
// The 12-byte typeinfo object is __tiP8ItemProp at 0x8eb2a8. It is COMMON, so
// its file image is zero. When its first word is zero, the function calls
// __tf8ItemProp and then __rtti_ptr with this object, the name "P8ItemProp"
// (0x461dc0), and __ti8ItemProp. A nonzero first word makes neither call. It
// returns the object.

extern "C" {
unsigned char norrath_ti_P8ItemProp[12];
void* __tf8ItemProp();
void norrath_rtti_ptr(void* typeinfo, const char* name, void* target);
extern unsigned char norrath_ti_8ItemProp[];
}

extern "C" void* __tfP8ItemProp() {
    unsigned char* raw = norrath_ti_P8ItemProp;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) {
        __tf8ItemProp();
        norrath_rtti_ptr(raw, "P8ItemProp", norrath_ti_8ItemProp);
    }
    return raw;
}
