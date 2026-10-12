// Observed __tf8AutoProp at 0x314ef0, 132 bytes.
// The 12-byte typeinfo object is __ti8AutoProp at 0x8eb518. Its first word
// gates everything. When it is nonzero the object is returned untouched.
// Otherwise the typeinfo object for 10AutoObject (__ti10AutoObject at
// 0x8eb4e8) is checked the same way: a zero first word runs the GameObject
// typeinfo function and then __rtti_si on it with the name "10AutoObject" and
// the GameObject typeinfo (gp 0x4879f0 minus 3112 is 0x486dc8). Finally
// __rtti_si runs on this object with the name "8AutoProp" and the
// 10AutoObject typeinfo as its base. No separate 10AutoObject typeinfo
// function is called; its object is set up inline here. Unknown: whether
// another unit also sets that object up.

extern "C" {
unsigned char norrath_ti_8AutoProp[12];
unsigned char norrath_ti_10AutoObject[12];
void* norrath_vt_tf_10GameObject();
void norrath_rtti_si(void* typeinfo, const char* name, void* base);
extern unsigned char norrath_ti_10GameObject[];
}

namespace {
unsigned first_word(const unsigned char* raw) {
    return unsigned(raw[0]) | (unsigned(raw[1]) << 8) | (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
}
}

extern "C" void* __tf8AutoProp() {
    if (first_word(norrath_ti_8AutoProp) == 0u) {
        if (first_word(norrath_ti_10AutoObject) == 0u) {
            norrath_vt_tf_10GameObject();
            norrath_rtti_si(norrath_ti_10AutoObject, "10AutoObject", norrath_ti_10GameObject);
        }
        norrath_rtti_si(norrath_ti_8AutoProp, "8AutoProp", norrath_ti_10AutoObject);
    }
    return norrath_ti_8AutoProp;
}
