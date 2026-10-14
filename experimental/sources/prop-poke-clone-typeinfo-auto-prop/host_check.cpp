#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>

extern "C" unsigned char norrath_ti_10GameObject[8] = {};

static int failures = 0;
static int base_calls = 0;
static int rtti_calls = 0;
static void* seen_object[4];
static const char* seen_name[4];
static void* seen_base[4];

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
extern "C" void* norrath_vt_tf_10GameObject() {
    ++base_calls;
    norrath_ti_10GameObject[0] = 1;
    return norrath_ti_10GameObject;
}
extern "C" void norrath_rtti_si(void* typeinfo, const char* name, void* base) {
    seen_object[rtti_calls] = typeinfo;
    seen_name[rtti_calls] = name;
    seen_base[rtti_calls] = base;
    ++rtti_calls;
    static_cast<unsigned char*>(typeinfo)[0] = 2;
}
static void reset() {
    base_calls = 0;
    rtti_calls = 0;
    std::memset(norrath_ti_8AutoProp, 0, 12);
    std::memset(norrath_ti_10AutoObject, 0, 12);
}
int main() {
    reset();
    void* first = __tf8AutoProp();
    expect(first == norrath_ti_8AutoProp, "returns the AutoProp typeinfo");
    expect(base_calls == 1 && rtti_calls == 2, "base once, two setups");
    expect(seen_object[0] == norrath_ti_10AutoObject && std::strcmp(seen_name[0], "10AutoObject") == 0 &&
           seen_base[0] == norrath_ti_10GameObject, "AutoObject first, based on GameObject");
    expect(seen_object[1] == norrath_ti_8AutoProp && std::strcmp(seen_name[1], "8AutoProp") == 0 &&
           seen_base[1] == norrath_ti_10AutoObject, "AutoProp second, based on AutoObject");
    expect(__tf8AutoProp() == first && base_calls == 1 && rtti_calls == 2, "set AutoProp skips everything");

    reset();
    norrath_ti_10AutoObject[0] = 7;
    expect(__tf8AutoProp() == norrath_ti_8AutoProp, "returns the AutoProp typeinfo");
    expect(base_calls == 0 && rtti_calls == 1 && seen_object[0] == norrath_ti_8AutoProp &&
           seen_base[0] == norrath_ti_10AutoObject, "set AutoObject skips its setup and the base call");

    reset();
    norrath_ti_8AutoProp[2] = 1;
    norrath_ti_10AutoObject[0] = 0;
    expect(__tf8AutoProp() == norrath_ti_8AutoProp && base_calls == 0 && rtti_calls == 0, "nonzero AutoProp word skips all");
    reset();
    norrath_ti_8AutoProp[8] = 5;
    __tf8AutoProp();
    expect(rtti_calls == 2, "only the first word gates");
    return failures == 0 ? 0 : 1;
}
