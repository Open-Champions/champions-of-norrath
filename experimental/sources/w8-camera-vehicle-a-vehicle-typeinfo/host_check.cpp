// Host contract for __tf7Vehicle. Not an EE plan input.
#include "typeinfo.cpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
static int base_calls = 0;
static int rtti_calls = 0;
static void* seen_object;
static const char* seen_name;
static const void* seen_base;
unsigned char __ti10GameObject[12];

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

extern "C" void* __tf10GameObject() {
    ++base_calls;
    return nullptr;
}
extern "C" void* __rtti_si(void* typeinfo, const char* name, const void* base) {
    ++rtti_calls;
    seen_object = typeinfo;
    seen_name = name;
    seen_base = base;
    static_cast<unsigned char*>(typeinfo)[0] = 2;
    return typeinfo;
}

int main() {
    std::memset(__ti7Vehicle, 0, 12);
    void* first = __tf7Vehicle();
    expect(first == __ti7Vehicle, "returns the typeinfo object");
    expect(base_calls == 1 && rtti_calls == 1, "zero first word runs the base initializer and __rtti_si once");
    expect(seen_object == __ti7Vehicle && seen_base == __ti10GameObject, "object and base passed to __rtti_si");
    expect(seen_name && std::strcmp(seen_name, "7Vehicle") == 0, "class name text");
    void* second = __tf7Vehicle();
    expect(second == first && base_calls == 1 && rtti_calls == 1, "a stored first word skips both calls");
    for (int byte = 0; byte < 4; ++byte) {
        base_calls = rtti_calls = 0;
        std::memset(__ti7Vehicle, 0, 12);
        __ti7Vehicle[byte] = 1;
        expect(__tf7Vehicle() == __ti7Vehicle && base_calls == 0 && rtti_calls == 0, "any nonzero byte of the first word skips initialization");
    }
    base_calls = rtti_calls = 0;
    std::memset(__ti7Vehicle, 0, 12);
    __ti7Vehicle[4] = 9;
    __ti7Vehicle[11] = 4;
    __tf7Vehicle();
    expect(base_calls == 1 && rtti_calls == 1, "only the first word gates initialization");
    return failures ? 1 : 0;
}
