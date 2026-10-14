#include "free_count.hpp"
#include "byte_view.hpp"
extern "C" int norrath_block_get_number_free() {
    using namespace norrath_block_view;
    const unsigned region = address(norrath_block_region_view);
    unsigned total = 0u;
    for (unsigned row = 0u; row != 3u; ++row) {
        unsigned node = read_word(region + row * 16u + 12u);
        unsigned count = 0u;
        while (node != 0u) {
            node = read_word(node);
            ++count;
        }
        total += count;
    }
    return signed_word(total);
}
