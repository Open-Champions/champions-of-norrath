#include "sound_register_semaphore.hpp"
using namespace norrath_sound_register_semaphore_detail;
extern "C" unsigned long long norrath_sound_get_system_register(int register_id) {
    // Original SLL/ADDU use wrapped low32 pointer arithmetic with no bounds,
    // negative-index or null guards. This is a read through the real external
    // view, not a synthesized kernel register cache or owned zero table.
    const Address offset=unsigned(register_id)<<2;
    const Address slot=address(norrath_sound_system_register_words_view)+offset;
    // Genuine signed LW accesses an aligned word (base is4-aligned, index*4).
    // Retain the original repeated polling access; do not hoist a cached value.
    const int value=*reinterpret_cast<const volatile int*>(
        static_cast<__UINTPTR_TYPE__>(slot));
    return Raw64(value); // signed32->unsigned64 modulo preserves LW sign bits
}
