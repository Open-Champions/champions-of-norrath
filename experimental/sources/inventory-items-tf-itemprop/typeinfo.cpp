// Observed __tf8ItemProp at 0x30dc58, 132 bytes.
// Three typeinfo objects are involved: __ti8ItemProp (0x8eade8, 12 bytes),
// __ti4Prop (0x8eadb8, 12 bytes) and __ti10GameObject (0x486dc8). The first two
// are COMMON, so their file image is zero. If the first word of __ti8ItemProp is
// zero: when the first word of __ti4Prop is also zero, the function calls
// __tf10GameObject and then __rtti_si(__ti4Prop, "4Prop" (0x483918),
// __ti10GameObject). In both sub-cases it then calls
// __rtti_si(__ti8ItemProp, "8ItemProp" (0x460cf0), __ti4Prop). A nonzero first
// word of __ti8ItemProp makes no calls. It returns __ti8ItemProp.
// Unknown: whether __tf4Prop is a separate function in the image; this one
// inlines the same initialization and does not call it.

extern "C" {
unsigned char norrath_ti_8ItemProp[12];
extern unsigned char norrath_ti_4Prop[];
void* norrath_vt_tf_10GameObject();
void norrath_rtti_si(void* typeinfo, const char* name, void* base);
extern unsigned char norrath_ti_10GameObject[];
}

namespace {
unsigned first_word(const unsigned char* raw) {
    return unsigned(raw[0]) | (unsigned(raw[1]) << 8) | (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
}
}

extern "C" void* __tf8ItemProp() {
    if (first_word(norrath_ti_8ItemProp) == 0u) {
        if (first_word(norrath_ti_4Prop) == 0u) {
            norrath_vt_tf_10GameObject();
            norrath_rtti_si(norrath_ti_4Prop, "4Prop", norrath_ti_10GameObject);
        }
        norrath_rtti_si(norrath_ti_8ItemProp, "8ItemProp", norrath_ti_4Prop);
    }
    return norrath_ti_8ItemProp;
}
