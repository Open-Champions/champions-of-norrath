#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" void norrath_vt_dtor_17charAmbient_Light(void* object, int flags);
extern "C" unsigned char norrath_factory_ambient_vtable[8] = {};

static int failures = 0;
static int base_calls = 0;
static int free_calls = 0;
static int base_flags = 99;
static void* base_object = nullptr;
static void* free_object = nullptr;
static unsigned vtable_seen_by_base = 0;
static bool free_before_base = false;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned pointer_word(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pointer));
}

extern "C" void norrath_vt_dtor_10GameObject(void* object, int flags) {
    ++base_calls;
    base_flags = flags;
    base_object = object;
    vtable_seen_by_base = read_word(static_cast<unsigned char*>(object) + 72);
}
extern "C" void norrath_block_free(void* object) {
    if (base_calls == 0) free_before_base = true;
    ++free_calls;
    free_object = object;
}

static void reset() {
    base_calls = 0;
    free_calls = 0;
    base_flags = 99;
    base_object = nullptr;
    free_object = nullptr;
    vtable_seen_by_base = 0;
    free_before_base = false;
}

int main() {
    unsigned char object[80];
    std::memset(object, 0x5a, sizeof object);
    const unsigned expect_vtable = pointer_word(norrath_factory_ambient_vtable);
    norrath_vt_dtor_17charAmbient_Light(object, 0);
    expect(base_calls == 1 && base_flags == 0 && base_object == object, "flag zero still runs the base destructor with zero");
    expect(vtable_seen_by_base == expect_vtable, "the class vtable is stored before the base destructor");
    expect(free_calls == 0 && !free_before_base, "flag zero does not free");

    reset();
    norrath_vt_dtor_17charAmbient_Light(object, 1);
    expect(base_calls == 1 && base_flags == 0 && free_calls == 1 && free_object == object && !free_before_base,
           "bit 0 frees after the base destructor");

    reset();
    norrath_vt_dtor_17charAmbient_Light(object, 2);
    expect(base_calls == 1 && free_calls == 0, "bit 1 does not free");

    reset();
    norrath_vt_dtor_17charAmbient_Light(object, 5);
    expect(free_calls == 1 && base_flags == 0, "bit 0 frees even when another bit is set");

    reset();
    norrath_vt_dtor_17charAmbient_Light(object, -1);
    expect(free_calls == 1 && base_object == object && base_flags == 0, "a negative flag still frees only because bit 0 is set");
    return failures == 0 ? 0 : 1;
}
