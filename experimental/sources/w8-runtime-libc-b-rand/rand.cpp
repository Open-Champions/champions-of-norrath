// Observed rand at 0x00302fd8, 100 bytes. The 64-bit state at offset 168 of
// the object _impure_ptr aims at is multiplied by 0x5851f42d4c957f2d (the
// original calls __muldi3 for this), incremented by one, and stored back.
// The result is bits 32..62 of the new state: the high word masked with
// 0x7fffffff, as a non-negative int.
#include "../libc-format-impure/impure_state.hpp"

extern "C" NorrathImpureState* norrath_factory_impure_pointer_word;

extern "C" int rand() {
    NorrathImpureState* state = norrath_factory_impure_pointer_word;
    const unsigned long long next = state->random_next * 0x5851f42d4c957f2dull + 1ull;
    state->random_next = next;
    return static_cast<int>(static_cast<unsigned>(next >> 32) & 0x7fffffffu);
}
