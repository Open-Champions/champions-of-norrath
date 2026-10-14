#include <cstdio>
#include <cstdint>
#include <cstring>

extern "C" void counter_destructor(void* self, int flags) asm("_$_7Counter");
extern "C" const unsigned char norrath_next_view_449160[4] = {};
unsigned long long norrath_vslots_a_host_base;

static int base_flags = -1;
static void* base_object = nullptr;
static int base_calls = 0;
static int frees = 0;
static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" void norrath_vt_dtor_10GameObject(void* object, int flags) {
    ++base_calls;
    base_object = object;
    base_flags = flags;
    unsigned stored = unsigned(static_cast<unsigned char*>(object)[72]) |
                      (unsigned(static_cast<unsigned char*>(object)[73]) << 8) |
                      (unsigned(static_cast<unsigned char*>(object)[74]) << 16) |
                      (unsigned(static_cast<unsigned char*>(object)[75]) << 24);
    unsigned wanted = static_cast<unsigned>(reinterpret_cast<uintptr_t>(norrath_next_view_449160) - norrath_vslots_a_host_base);
    expect(stored == wanted, "vtable stored before the base destructor");
}
extern "C" void norrath_block_free(void* object) {
    ++frees;
    expect(object == base_object, "frees the same object");
}
int main() {
    alignas(16) unsigned char object[80];
    norrath_vslots_a_host_base = reinterpret_cast<uintptr_t>(object) - 64;
    std::memset(object, 0x3c, sizeof object);
    counter_destructor(object, 0);
    expect(base_calls == 1 && base_flags == 0 && frees == 0, "flag 0 calls the base and does not free");
    counter_destructor(object, 2);
    expect(base_calls == 2 && frees == 0, "flag 2 does not free");
    counter_destructor(object, 1);
    expect(frees == 1, "flag 1 frees");
    counter_destructor(object, 5);
    expect(frees == 2 && base_calls == 4, "any odd flag frees after the base call");
    return failures ? 1 : 0;
}
