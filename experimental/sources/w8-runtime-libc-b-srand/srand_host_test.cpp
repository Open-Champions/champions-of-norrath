// Host check only; not an EE build input.
#include "../libc-format-impure/impure_state.hpp"
#include <cstdio>

extern "C" void srand(unsigned seed);
extern "C" NorrathImpureState* norrath_factory_impure_pointer_word = 0;

int main() {
    static NorrathImpureState state;
    norrath_factory_impure_pointer_word = &state;
    state.random_next = 0xffffffffffffffffull;
    srand(0x80000001u);
    if (state.random_next != 0x0000000080000001ull) return 1;
    srand(0);
    if (state.random_next != 0) return 2;
    srand(0xffffffffu);
    if (state.random_next != 0x00000000ffffffffull) return 3;
    std::puts("srand ok");
    return 0;
}
