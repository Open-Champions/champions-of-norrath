// Host contract for __tf11LavaTractor. Not an EE plan input.
#include "typeinfo.cpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
static int base_calls = 0;
static int rtti_calls = 0;
static void* seen_object;
static const char* seen_name;
static const void* seen_base;
unsigned char __ti11LavaVehicle[12];

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

extern "C" void* __tf11LavaVehicle() {
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
    std::memset(__ti11LavaTractor, 0, 12);
    void* first = __tf11LavaTractor();
    expect(first == __ti11LavaTractor, "returns the typeinfo object");
    expect(base_calls == 1 && rtti_calls == 1, "zero first word runs the base initializer and __rtti_si once");
    expect(seen_object == __ti11LavaTractor && seen_base == __ti11LavaVehicle, "object and base passed to __rtti_si");
    expect(seen_name && std::strcmp(seen_name, "11LavaTractor") == 0, "class name text");
    void* second = __tf11LavaTractor();
    expect(second == first && base_calls == 1 && rtti_calls == 1, "a stored first word skips both calls");
    for (int byte = 0; byte < 4; ++byte) {
        base_calls = rtti_calls = 0;
        std::memset(__ti11LavaTractor, 0, 12);
        __ti11LavaTractor[byte] = 1;
        expect(__tf11LavaTractor() == __ti11LavaTractor && base_calls == 0 && rtti_calls == 0, "any nonzero byte of the first word skips initialization");
    }
    base_calls = rtti_calls = 0;
    std::memset(__ti11LavaTractor, 0, 12);
    __ti11LavaTractor[4] = 9;
    __ti11LavaTractor[11] = 4;
    __tf11LavaTractor();
    expect(base_calls == 1 && rtti_calls == 1, "only the first word gates initialization");
    return failures ? 1 : 0;
}
