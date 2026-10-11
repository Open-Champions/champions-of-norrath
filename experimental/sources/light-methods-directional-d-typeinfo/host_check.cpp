#include <cstdio>
#include <cstring>

extern "C" void* norrath_vt_tf_22charDirectional_LightD();
extern "C" unsigned char norrath_ti_22charDirectional_LightD[12];
extern "C" unsigned char norrath_ti_10GameObject[8] = {};

static int failures = 0;
static int base_calls = 0;
static int rtti_calls = 0;
static void* seen_object = nullptr;
static const char* seen_name = nullptr;
static void* seen_base = nullptr;

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
    ++rtti_calls;
    seen_object = typeinfo;
    seen_name = name;
    seen_base = base;
    static_cast<unsigned char*>(typeinfo)[0] = 2;
}

int main() {
    std::memset(norrath_ti_22charDirectional_LightD, 0, 12);
    std::memset(norrath_ti_10GameObject, 0, 8);
    void* first = norrath_vt_tf_22charDirectional_LightD();
    expect(first == norrath_ti_22charDirectional_LightD, "returns the typeinfo object");
    expect(base_calls == 1, "initializes the GameObject typeinfo first");
    expect(rtti_calls == 1 && seen_object == norrath_ti_22charDirectional_LightD, "passes this typeinfo to single-inheritance setup");
    expect(seen_base == norrath_ti_10GameObject, "passes the GameObject typeinfo as the base");
    expect(seen_name && std::strcmp(seen_name, "22charDirectional_LightD") == 0, "passes the observed class name");

    void* second = norrath_vt_tf_22charDirectional_LightD();
    expect(second == first && base_calls == 1 && rtti_calls == 1, "a stored first word skips both calls");

    base_calls = 0;
    rtti_calls = 0;
    std::memset(norrath_ti_22charDirectional_LightD, 0, 12);
    norrath_ti_22charDirectional_LightD[1] = 1;
    void* third = norrath_vt_tf_22charDirectional_LightD();
    expect(third == norrath_ti_22charDirectional_LightD && base_calls == 0 && rtti_calls == 0, "a nonzero second byte is part of the gate word");

    std::memset(norrath_ti_22charDirectional_LightD, 0, 12);
    norrath_ti_22charDirectional_LightD[4] = 9;
    norrath_ti_22charDirectional_LightD[11] = 4;
    void* fourth = norrath_vt_tf_22charDirectional_LightD();
    expect(fourth == norrath_ti_22charDirectional_LightD && base_calls == 1 && rtti_calls == 1, "only the first word gates initialization");
    return failures == 0 ? 0 : 1;
}
