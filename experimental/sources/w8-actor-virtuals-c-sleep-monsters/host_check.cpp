#define NORRATH_W8C_HOST 1
#include "sleep_monsters.cpp"
#include "../w8-actor-virtuals-c-bytes/host_vcall.hpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
static void (*seen)(void*) = nullptr;
static int slot_calls = 0, destroy_calls = 0;
static void* slot_self = nullptr;
static int slot_arg = -1;
static void* destroyed = nullptr;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" void objectIterateRunLists__FPFP10GameObject_v(void (*callback)(void*)) { seen = callback; }
extern "C" void norrath_list_destroy(void* head) { ++destroy_calls; destroyed = head; }
static void slot(void* self, int arg) { ++slot_calls; slot_self = self; slot_arg = arg; }

struct Case { unsigned flags; unsigned char mark; short mode; };
static void run(const Case& c, bool expect_slot, bool expect_destroy, unsigned char expect_mark) {
    unsigned char object[800];
    unsigned char table[256];
    std::memset(object, 0x5a, sizeof object);
    std::memset(table, 0, sizeof table);
    w8c::host_install_slot(object, table, 184u, 8, reinterpret_cast<void*>(slot));
    w8c::write_word(w8c::address(object) + 12u, c.flags);
    object[570] = c.mark;
    w8c::write_u16(w8c::address(object) + 572u, static_cast<unsigned short>(c.mode));
    slot_calls = destroy_calls = 0;
    seen(object);
    expect((slot_calls == 1) == expect_slot, "slot call");
    expect((destroy_calls == 1) == expect_destroy, "list destroy");
    if (expect_slot) expect(slot_self == object + 8 && slot_arg == 0, "slot receives adjusted object and zero");
    if (expect_destroy) expect(destroyed == object + 576, "destroys list at 576");
    expect(object[570] == expect_mark, "byte 570");
    expect(w8c::read_word(w8c::address(object) + 576u) == 0x5a5a5a5au, "head word left alone");
}
int main() {
    sleepMonsters__8creaturev();
    expect(seen != nullptr, "callback passed");
    run({0x20u, 0u, 3}, true, true, 0x10u);
    run({0x20u, 0u, 10}, false, true, 0x10u);
    run({0x20u, 0u, static_cast<short>(0xfff6)}, true, true, 0x10u);
    run({0x20u, 0u, 0x100a}, true, true, 0x10u);
    run({0x20u, 1u, 3}, false, false, 1u);
    run({0x20u, 0x80u, 3}, false, false, 0x80u);
    run({0x60u, 0u, 3}, false, false, 0u);
    run({0x00u, 0u, 3}, false, false, 0u);
    run({0x40u, 0u, 3}, false, false, 0u);
    return failures ? 1 : 0;
}
