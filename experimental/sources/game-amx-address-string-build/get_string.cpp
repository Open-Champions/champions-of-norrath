#include "address_string.hpp"
extern "C" unsigned long long norrath_amx_get_string(
    char* destination, const void* cells) {
    using namespace norrath_constructor_detail;
    Address output = address(destination);
    Address source = address(cells);
    const unsigned initial = read_word(source);
    if (signed_word(initial) >= 256) {
        unsigned byte_index = 3u;
        unsigned captured = 0u;
        for (;;) {
            // First packed word is deliberately reread after mode selection.
            if (byte_index == 3u) {
                captured = read_word(source);
                source += 4u;
            }
            const unsigned char character = static_cast<unsigned char>(
                captured >> (byte_index * 8u));
            byte_index = (byte_index + 3u) & 3u;
            *bytes(output) = character;
            output += 1u;
            if (character == 0u) return 0ull;
        }
    }
    if (initial != 0u) {
        unsigned next;
        do {
            const unsigned char character = *bytes(source);
            source += 4u;
            *bytes(output) = character;
            // Continuation tests the next entire cell, not its low byte.
            next = read_word(source);
            output += 1u;
        } while (next != 0u);
    }
    *bytes(output) = 0u;
    return 0ull;
}
