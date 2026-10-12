#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>

extern "C" unsigned char norrath_ti_6PhProp[12] = {};

static int failures = 0;
static int target_calls = 0;
static int rtti_calls = 0;
static void* seen_object = nullptr;
static const char* seen_name = nullptr;
static void* seen_target = nullptr;
static int order = 0;
static int target_order = 0;
static int rtti_order = 0;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
extern "C" void* __tf6PhProp() {
    ++target_calls;
    target_order = ++order;
    return norrath_ti_6PhProp;
}
extern "C" void norrath_rtti_ptr(void* typeinfo, const char* name, void* target) {
    ++rtti_calls;
    rtti_order = ++order;
    seen_object = typeinfo;
    seen_name = name;
    seen_target = target;
    static_cast<unsigned char*>(typeinfo)[0] = 2;
}

int main() {
    std::memset(norrath_ti_P6PhProp, 0, 12);
    void* first = __tfP6PhProp();
    expect(first == norrath_ti_P6PhProp, "returns the pointer typeinfo object");
    expect(target_calls == 1 && rtti_calls == 1 && target_order < rtti_order, "target typeinfo function runs before the pointer setup");
    expect(seen_object == norrath_ti_P6PhProp && seen_target == norrath_ti_6PhProp, "pointer setup gets this object and the target typeinfo");
    expect(seen_name && std::strcmp(seen_name, "P6PhProp") == 0, "passes the observed class name");

    expect(__tfP6PhProp() == first && target_calls == 1 && rtti_calls == 1, "a stored first word skips both calls");

    target_calls = 0;
    rtti_calls = 0;
    std::memset(norrath_ti_P6PhProp, 0, 12);
    norrath_ti_P6PhProp[3] = 1;
    expect(__tfP6PhProp() == norrath_ti_P6PhProp && target_calls == 0 && rtti_calls == 0, "a nonzero high byte is part of the gate word");

    std::memset(norrath_ti_P6PhProp, 0, 12);
    norrath_ti_P6PhProp[4] = 9;
    norrath_ti_P6PhProp[11] = 4;
    expect(__tfP6PhProp() == norrath_ti_P6PhProp && target_calls == 1 && rtti_calls == 1, "only the first word gates the calls");
    return failures == 0 ? 0 : 1;
}
