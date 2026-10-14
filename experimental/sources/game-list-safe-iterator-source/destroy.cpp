#include "safe_iterator.hpp"
#include "iterator_memory.hpp"
using namespace norrath_safe_iterator_detail;

extern "C" void norrath_list_safe_iterator_destroy(unsigned char* iterator, int flags) {
    const Address marker = address(iterator);
    const Address successor = read_word(marker);
    if (successor != 0u) {
        const Address previous = read_word(marker + 4u);
        write_word(successor + 4u, previous);
    }
    const Address previous = read_word(marker + 4u);
    if (previous != 0u) {
        const Address next = read_word(marker);
        write_word(previous, next);
    }
    if ((static_cast<unsigned>(flags) & 1u) != 0u)
        norrath_animation_state_deallocate(iterator);
}
