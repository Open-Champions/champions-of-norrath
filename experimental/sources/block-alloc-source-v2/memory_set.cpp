#include "byte_view.hpp"
extern "C" void* norrath_block_memory_set(void* destination, int value, unsigned byte_count) {
    using namespace norrath_block_view;
    unsigned cursor = address(destination);
    const unsigned char byte = static_cast<unsigned char>(unsigned(value) & 255u);
    // Complete original memset value/effect contract for ordinary valid spans.
    // Original optimized128/64-bit writes and scheduling are not a readtrace claim.
    while (byte_count != 0u) {
        *bytes(cursor) = byte;
        ++cursor;
        --byte_count;
    }
    return destination;
}
