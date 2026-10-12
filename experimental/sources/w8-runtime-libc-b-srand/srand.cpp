// Observed srand at 0x00302fc0, 24 bytes. It loads the pointer stored at
// _impure_ptr (0x00448480) and writes the argument, zero-extended to 64 bits,
// at offset 168 of the object it aims at. The initial image holds the
// generator seed 1 there. Nothing is returned.
#include "../libc-format-impure/impure_state.hpp"

extern "C" NorrathImpureState* norrath_factory_impure_pointer_word;

extern "C" void srand(unsigned seed) {
    norrath_factory_impure_pointer_word->random_next = seed;
}
