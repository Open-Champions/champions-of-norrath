#include "byte_view.hpp"
extern "C" unsigned char* norrath_block_alloc(int requested_size) {
    using namespace norrath_block_view;
    const unsigned region = address(norrath_block_region_view);
    unsigned index = 0;
    unsigned row;
    for (;;) {
        row = region + (index << 4);
        const int capacity = signed_word(read_word(row));
        if (capacity >= requested_size && read_word(row + 12u) != 0u) break;
        ++index;
    }
    // The actual function tests index==3 AFTER selecting a nonempty row.
    // It does not use row3 as an unconditional stopping sentinel.
    if (index == 3u) return nullptr;
    const unsigned byte_count = read_word(row);
    const unsigned head = read_word(row + 12u);
    const unsigned next = read_word(head);
    write_word(row + 12u, next);
    unsigned char* const object = bytes(head);
    norrath_block_memory_set(object, 0, byte_count);
    norrath_list_set_copy_object(object);
    return object;
}
