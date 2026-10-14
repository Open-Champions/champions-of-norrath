// Host contract for _$_11LavaTractor. Not an EE plan input.
#include "destructor.cpp"

#include <cstdint>
#include <cstdio>
#include <cstring>

static int failures = 0;
static int base_calls = 0, free_calls = 0, base_flags = 99;
static void* base_object;
static void* free_object;
static unsigned vtable_seen_by_base;
static int free_after_base;
unsigned char _vt$11LavaVehicle[248];

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

extern "C" void _$_7Vehicle(void* object, int flags) {
    ++base_calls;
    base_object = object;
    base_flags = flags;
    std::memcpy(&vtable_seen_by_base, static_cast<unsigned char*>(object) + 72, 4);
}
extern "C" void blockFree__FPv(void* object) {
    ++free_calls;
    free_object = object;
    free_after_base = base_calls == 1;
}

int main() {
    static unsigned char object[2304];
    const int flags[] = {0, 1, 2, 3, 0x10, 0x11, -1, -2, 0x7fffffff};
    for (int f : flags) {
        std::memset(object, 0x6b, sizeof object);
        unsigned char before[sizeof object];
        std::memcpy(before, object, sizeof object);
        base_calls = free_calls = 0;
        base_flags = 99;
        _$_11LavaTractor(object, f);
        const unsigned want_vt = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(_vt$11LavaVehicle));
        unsigned stored;
        std::memcpy(&stored, object + 72, 4);
        expect(stored == want_vt, "class vtable stored at +72");
        expect(base_calls == 1 && base_object == object && base_flags == 0, "Vehicle destructor called once with flags 0");
        expect(vtable_seen_by_base == want_vt, "vtable is stored before the Vehicle destructor runs");
        expect(free_calls == ((f & 1) ? 1 : 0), "block freed exactly when bit 0 is set");
        if (f & 1) expect(free_object == object && free_after_base, "freed after the Vehicle destructor");
        std::memcpy(before + 72, object + 72, 4);
        expect(std::memcmp(before, object, sizeof object) == 0, "no other byte changes here");
    }
    return failures ? 1 : 0;
}
