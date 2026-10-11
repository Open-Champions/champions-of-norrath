#include "initialization.hpp"
#include "byte_view.hpp"
extern "C" void norrath_block_alloc_init() {
    using namespace norrath_block_view;
    const unsigned pool = address(norrath_block_data_pool_view);
    norrath_block_memory_set(bytes(pool), 0, 0x000e7700u);
    const unsigned region = address(norrath_block_region_view);
    unsigned cursor = pool;
    for (unsigned index = 0; index != 3u; ++index) {
        const unsigned row = region + (index << 4);
        const unsigned count = read_word(row + 4u);
        const unsigned capacity = read_word(row);
        unsigned node = cursor;
        const unsigned initial_remaining = count - 1u;
        write_word(row + 8u, cursor);
        write_word(row + 12u, node);
        cursor += capacity * count;  // Original signed MULT low32 result.
        if (signed_word(initial_remaining) > 0) {
            unsigned remaining = read_word(row + 4u) - 1u;
            const unsigned stride = read_word(row);
            do {
                const unsigned next = node + stride;
                --remaining;
                write_word(node, next);
                node = next;
            } while (remaining != 0u);
        }
        write_word(node, 0u);
    }
    write_word(region + 56u, cursor);
    // The original final row's nonzero head is poolbase, NOT a fourth
    // newly allocated list. blockAlloc selects this row then returnsnull.
    write_word(region + 60u, pool);
}
