// Host contract for animFindComponentWithFlags__FP14AnimationStatei. Not an EE plan input.
// Build: c++ -std=c++17 -DNORRATH_HOST_ARENA host_check.cpp
#define NORRATH_HOST_ARENA
#include "../w9-animation-object-a-common/host_arena.hpp"
#include "../w9-animation-object-a-common/host_check.hpp"
#include "anim_find_component_with_flags.cpp"
int main() {
    unsigned char* s = arena_at(0x100);
    unsigned char before[8 + 32 * 8];
    int failures = 0;
    auto fill = [&](unsigned count, const unsigned* flags, unsigned n) {
        std::memset(s, 0x00, sizeof before);
        s[4] = (unsigned char)count;
        for (unsigned i = 0; i != 8; ++i) {
            sw_(s + 8 + 32 * i, 0x1000 + i);       // header slots: must not matter
            sh_(s + 8 + 32 * i + 28, i < n ? flags[i] : 0xffff);  // beyond-count slots match everything
        }
        std::memcpy(before, s, sizeof before);
    };
    auto expect = [&](const char* what, int mask, int want) {
        const int got = animFindComponentWithFlags__FP14AnimationStatei(s, mask);
        if (got != want) { std::printf("%s: got %d want %d\n", what, got, want); ++failures; }
        if (same(what, s, before, sizeof before)) ++failures;
    };
    const unsigned f1[] = {0x0001, 0x0010, 0x0110, 0x8000};
    fill(4, f1, 4);
    expect("first", 1, 0);
    expect("second", 0x10, 1);
    expect("first of several", 0x100, 2);
    expect("two bits, first hit", 0x0101, 0);
    expect("sign bit", 0x8000, 3);
    expect("sign bit as negative mask", (int)0xffff8000, 3);
    expect("none", 0x4000, -1000000000);
    expect("zero mask", 0, -1000000000);
    // The count limits the scan: slot 4.. match everything but are beyond the count.
    fill(2, f1, 4);
    expect("beyond count", 0x8000, -1000000000);
    expect("inside count", 0x10, 1);
    fill(0, f1, 4);
    expect("count zero", 1, -1000000000);
    fill(0x80, f1, 4);  // signed byte -128
    expect("negative count", 1, -1000000000);
    fill(0xff, f1, 4);
    expect("count -1", 1, -1000000000);
    fill(8, f1, 4);
    expect("fifth slot", 0x4, 4);
    if (failures) return 1;
    std::printf("animFindComponentWithFlags check passed\n");
    return 0;
}
