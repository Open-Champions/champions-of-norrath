#include "name_pool.hpp"

extern "C" char* norrath_megatile_add_pm_list(char* input) {
    using namespace norrath_name_pool_detail;
    const Address source = address(input);
    const Address pool = address(norrath_megatile_pm_characters_view);
    const Address used_word = address(norrath_megatile_counters_view) + 8u;
    const long long scan_limit = signed_word(read_word(used_word));
    unsigned source_offset = 0u;

    // The original scans every pool byte, including interior name positions.
    // A mismatch resets the source offset without retrying this pool byte.
    for (unsigned pool_offset = 0u; signed_word(pool_offset) < scan_limit;
         pool_offset += 1u) {
        const Address current_source = source + source_offset;
        const unsigned first = *bytes(current_source);
        source_offset += 1u;
        if (first == 0u) {
            const unsigned pool_byte = *bytes(pool + pool_offset);
            if (pool_byte == 0u) {
                const unsigned length = norrath_factory_strlen(input);
                return text(pool + (pool_offset - length));
            }
        }
        // Both the source and the pool are reread on the comparison path.
        const unsigned current = *bytes(current_source);
        const unsigned candidate = *bytes(pool + pool_offset);
        if (current != candidate) source_offset = 0u;
    }

    // Helper calls may alter or alias live state. Keep all three fresh counter
    // reads, and capture the returned start after strcpy but before strlen.
    const unsigned destination_offset = read_word(used_word);
    norrath_factory_strcpy(text(pool + destination_offset), input);
    const unsigned returned_offset = read_word(used_word);
    const unsigned length = norrath_factory_strlen(input);
    const unsigned fresh_used = read_word(used_word);
    write_word(used_word, fresh_used + length + 1u);
    return text(pool + returned_offset);
}
