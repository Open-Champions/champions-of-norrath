#include "safe_iterator.hpp"
#include "iterator_memory.hpp"
using namespace norrath_safe_iterator_detail;

extern "C" void norrath_list_safe_iterator_next(unsigned char* iterator) {
    const Address marker = address(iterator);
    Address selected = read_word(marker);
    Address old_successor = selected;
    while (selected != 0u) {
        const Address object = read_word(selected + 12u);
        if (object != 0u) {
            const unsigned object_flags = read_word(object + 12u);
            const unsigned exclusion_mask = read_word(marker + 20u);
            if ((object_flags & exclusion_mask) == 0u) break;
        }
        selected = read_word(selected);
    }

    Address previous = read_word(marker + 4u);
    if (old_successor != 0u) {
        write_word(old_successor + 4u, previous);
        old_successor = read_word(marker);
        previous = read_word(marker + 4u);
    }
    // The original requires an active marker here, including the final advance.
    write_word(previous, old_successor);
    if (selected != 0u) {
        insert_after(marker, selected);
    } else {
        write_word(marker + 4u, 0u);
        write_word(marker, 0u);
    }
}
