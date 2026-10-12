// Host check only; not an EE build input.
#include "../libc-format-impure/impure_state.hpp"
#include <cstdio>

extern "C" int rand();
extern "C" NorrathImpureState* norrath_factory_impure_pointer_word = 0;

int main() {
    static NorrathImpureState state;
    norrath_factory_impure_pointer_word = &state;
    // Independent reference: the first outputs from seed 1 computed with
    // 128-bit arithmetic, and a long walk compared step by step.
    unsigned __int128 wide = 1;
    state.random_next = 1;
    // First value from seed 1: 0x5851f42d4c957f2e >> 32 = 0x5851f42d.
    if (rand() != 0x5851f42d) return 1;
    if (state.random_next != 0x5851f42d4c957f2eull) return 2;
    unsigned long long expect = 1;
    state.random_next = 1;
    bool saw_high_bit = false;
    for (int i = 0; i < 100000; ++i) {
        wide = (wide * 0x5851f42d4c957f2dull + 1) & ~0ull;
        expect = static_cast<unsigned long long>(wide);
        const int want = static_cast<int>((expect >> 32) & 0x7fffffffu);
        const int got = rand();
        if (got != want || got < 0) return 3;
        if (state.random_next != expect) return 4;
        if ((expect >> 63) != 0) saw_high_bit = true;
    }
    if (!saw_high_bit) return 5;
    // Seeds set by srand begin at any 32-bit value.
    state.random_next = 0xffffffffull;
    const unsigned long long step = 0xffffffffull * 0x5851f42d4c957f2dull + 1ull;
    if (rand() != static_cast<int>((step >> 32) & 0x7fffffffu)) return 6;
    std::puts("rand ok");
    return 0;
}
