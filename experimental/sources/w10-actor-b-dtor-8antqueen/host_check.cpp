// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST
#include <cstdio>
#include <cstring>
#include <string>

extern "C" const unsigned char norrath_ctors_ant_lantern_view_44c440[8] = {};
static std::string calls;
static unsigned char* current;
static void *base_object, *freed_object;
static int base_flags;
static unsigned vptr_at_base;
extern "C" void base_destructor(void* object, int flags) asm("_$_8Creature");
extern "C" void base_destructor(void* object, int flags) {
    calls += "base;";
    base_object = object; base_flags = flags;
    std::memcpy(&vptr_at_base, current + 72, 4);
}
extern "C" void norrath_block_free(void* object) { calls += "free;"; freed_object = object; }
#include "dtor.cpp"

int main() {
    int failures = 0;
    alignas(8) unsigned char object[512];
    const int flags_list[] = {0, 1, 2, 3, -2, -1, 0x10001, 0x100, static_cast<int>(0x80000000u)};
    for (int flags : flags_list) {
        std::memset(object, 0x5c, sizeof object);
        current = object; calls.clear(); base_object = freed_object = nullptr; vptr_at_base = 0;
        dtor_8AntQueen(object, flags);
        const bool frees = (flags & 1) != 0;
        if (calls != (frees ? "base;free;" : "base;")) { std::fprintf(stderr, "fail: calls %s for flags %x\n", calls.c_str(), flags); ++failures; }
        if (base_object != object || base_flags != 0) { std::fprintf(stderr, "fail: base arguments\n"); ++failures; }
        if (vptr_at_base != rtti_c::address_bits(norrath_ctors_ant_lantern_view_44c440)) { std::fprintf(stderr, "fail: vtable not stored before the base call\n"); ++failures; }
        if (frees && freed_object != object) { std::fprintf(stderr, "fail: freed object\n"); ++failures; }
        if (object[71] != 0x5c || object[76] != 0x5c || object[0] != 0x5c) { std::fprintf(stderr, "fail: neighbours\n"); ++failures; }
    }
    return failures ? 1 : 0;
}
