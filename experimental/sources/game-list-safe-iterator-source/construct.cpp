#include "safe_iterator.hpp"
#include "iterator_memory.hpp"
using namespace norrath_safe_iterator_detail;

extern "C" void norrath_list_safe_iterator_construct(
    unsigned char* iterator, unsigned char* head, int mask) {
    const Address marker = address(iterator);
    Address selected = read_word(address(head));
    const unsigned initial_mask = static_cast<unsigned>(mask);
    write_word(marker + 20u, initial_mask);

    bool first_candidate = true;
    while (selected != 0u) {
        const Address object = read_word(selected + 12u);
        if (object != 0u) {
            const unsigned object_flags = read_word(object + 12u);
            // Only the first candidate uses the incoming register mask.
            const unsigned exclusion_mask = first_candidate
                ? initial_mask : read_word(marker + 20u);
            if ((object_flags & exclusion_mask) == 0u) break;
        }
        selected = read_word(selected);
        first_candidate = false;
    }

    if (selected != 0u) {
        insert_after(marker, selected);
    } else {
        write_word(marker, 0u);
        write_word(marker + 4u, 0u);
    }
    write_word(marker + 12u, 0u);
}
