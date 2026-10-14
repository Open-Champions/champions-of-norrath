#define NORRATH_W8C_HOST 1
#include "wake_monsters.cpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
static int iterations = 0;
static void (*seen)(void*) = nullptr;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" void objectIterateRunLists__FPFP10GameObject_v(void (*callback)(void*)) {
    ++iterations;
    seen = callback;
}
static unsigned char run_case(unsigned flags, unsigned char before) {
    unsigned char object[600];
    std::memset(object, 0x5a, sizeof object);
    w8c::write_word(w8c::address(object) + 12u, flags);
    object[570] = before;
    seen(object);
    for (unsigned i = 0; i < sizeof object; ++i) {
        const bool written = i == 570u || (i >= 12u && i < 16u);
        if (!written && object[i] != 0x5a) { std::fprintf(stderr, "fail: stray write at %u\n", i); ++failures; }
    }
    return object[570];
}
int main() {
    wakeMonsters__8creaturev();
    expect(iterations == 1 && seen != nullptr, "one iteration with a callback");
    expect(run_case(0x20u, 0xffu) == 0xefu, "0x20 alone clears bit 0x10");
    expect(run_case(0x20u, 0x10u) == 0x00u, "only the 0x10 bit changes");
    expect(run_case(0x60u, 0xffu) == 0xffu, "0x40 blocks");
    expect(run_case(0x00u, 0xffu) == 0xffu, "missing 0x20 blocks");
    expect(run_case(0x40u, 0x10u) == 0x10u, "0x40 without 0x20 blocks");
    expect(run_case(0xffffffdfu, 0x1fu) == 0x1fu, "all but 0x20 blocks");
    expect(run_case(0x21u, 0x7fu) == 0x6fu, "other flag bits are ignored");
    return failures ? 1 : 0;
}
