#include <cstring>

extern "C" void* __tf8ItemProp();
extern "C" unsigned char norrath_ti_8ItemProp[12];
extern "C" unsigned char norrath_ti_4Prop[12] = {};
extern "C" unsigned char norrath_ti_10GameObject[8] = {};

static int base_calls, rtti_calls;
static void* objects[4];
static const char* names[4];
static void* bases[4];

extern "C" void* norrath_vt_tf_10GameObject() { ++base_calls; return norrath_ti_10GameObject; }
extern "C" void norrath_rtti_si(void* typeinfo, const char* name, void* base) {
    objects[rtti_calls] = typeinfo;
    names[rtti_calls] = name;
    bases[rtti_calls] = base;
    ++rtti_calls;
    static_cast<unsigned char*>(typeinfo)[0] = 2;
}

int main() {
    // Both words zero: the Prop typeinfo is initialized first, then ItemProp.
    void* first = __tf8ItemProp();
    if (first != norrath_ti_8ItemProp) return 1;
    if (base_calls != 1 || rtti_calls != 2) return 2;
    if (objects[0] != norrath_ti_4Prop || bases[0] != norrath_ti_10GameObject || std::strcmp(names[0], "4Prop") != 0) return 3;
    if (objects[1] != norrath_ti_8ItemProp || bases[1] != norrath_ti_4Prop || std::strcmp(names[1], "8ItemProp") != 0) return 4;
    if (__tf8ItemProp() != first || base_calls != 1 || rtti_calls != 2) return 5;
    // Prop already set up: only the ItemProp call is made.
    std::memset(norrath_ti_8ItemProp, 0, 12);
    base_calls = rtti_calls = 0;
    __tf8ItemProp();
    if (base_calls != 0 || rtti_calls != 1 || objects[0] != norrath_ti_8ItemProp) return 6;
    // Prop not set up and ItemProp already set: no calls at all.
    std::memset(norrath_ti_4Prop, 0, 12);
    base_calls = rtti_calls = 0;
    __tf8ItemProp();
    if (base_calls != 0 || rtti_calls != 0 || norrath_ti_4Prop[0] != 0) return 7;
    return 0;
}
