#include "free_count.hpp"
#include "byte_view.hpp"
extern "C" void norrath_block_free(void* object) {
    using namespace norrath_block_view;
    const unsigned pointer = address(object);
    const unsigned region = address(norrath_block_region_view);
    unsigned offset = 0u;
    // Actual row1 start determines the first class. The original advances
    // through later starts without a null/alignment/row-count guard.
    while (!(pointer < read_word(region + offset + 24u))) offset += 16u;
    const unsigned head_address = region + offset + 12u;
    const unsigned prior_head = read_word(head_address);
    write_word(head_address, pointer);
    write_word(pointer, prior_head);
}
// blockFree__FPv is this body: 0x001447d8, 84 bytes in the pinned image. The
// original picks the row from the same start words and pushes onto the same list.
extern "C" void blockFree__FPv(void* object) __attribute__((alias("norrath_block_free")));
