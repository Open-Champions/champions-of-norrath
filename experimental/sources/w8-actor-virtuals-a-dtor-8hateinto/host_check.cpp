#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" void dtor_8HateInto(void* object, int flags) asm("_$_8HateInto");
extern "C" unsigned char norrath_actors_b_view_44f150[8] = {};
extern "C" void autoprop_destructor(void* object, int flags) asm("_$_8AutoProp");

static int failures = 0;
static int member_calls = 0;
static void* member_object = nullptr;
static int member_flags = 99;
static int member_before_base = 0;
extern "C" void fx_creature_state_destructor(void* object, int flags) asm("_$_15FxCreatureState");
extern "C" void fx_creature_state_destructor(void* object, int flags) {
    ++member_calls;
    member_object = object;
    member_flags = flags;
}
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

extern "C" void autoprop_destructor(void* object, int flags) {
    if (member_calls == 1) member_before_base = 1;
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
    member_calls = 0;
    member_object = nullptr;
    member_flags = 99;
    member_before_base = 0;
}

int main() {
    unsigned char object[80];
    unsigned char before[80];
    std::memset(object, 0x5a, sizeof object);
    std::memcpy(before, object, sizeof object);
    const unsigned expect_vtable = pointer_word(norrath_actors_b_view_44f150);
    dtor_8HateInto(object, 0);
    expect(base_calls == 1 && base_flags == 0 && base_object == object, "flag zero still runs the base destructor with zero");
    expect(vtable_seen_by_base == expect_vtable, "the class vtable is stored before the base destructor");
    expect(free_calls == 0 && !free_before_base, "flag zero does not free");
    expect(member_calls == 1 && member_object == object + 408 && member_flags == 2 && member_before_base == 1,
           "the member at +408 is destroyed with flag 2 before the base destructor");
    expect(std::memcmp(object, before, 72) == 0 && std::memcmp(object + 76, before + 76, 4) == 0, "only the word at +72 is written");

    reset();
    dtor_8HateInto(object, 1);
    expect(base_calls == 1 && base_flags == 0 && free_calls == 1 && free_object == object && !free_before_base,
           "bit 0 frees after the base destructor");

    reset();
    dtor_8HateInto(object, 1);
    expect(member_calls == 1 && member_flags == 2, "the member flag stays 2 whatever the incoming flag");

    reset();
    dtor_8HateInto(object, 2);
    expect(base_calls == 1 && free_calls == 0, "bit 1 does not free");

    reset();
    dtor_8HateInto(object, 5);
    expect(free_calls == 1 && base_flags == 0, "bit 0 frees even when another bit is set");

    reset();
    dtor_8HateInto(object, -1);
    expect(free_calls == 1 && base_object == object && base_flags == 0, "negative flag frees because bit 0 is set");
    return failures == 0 ? 0 : 1;
}
