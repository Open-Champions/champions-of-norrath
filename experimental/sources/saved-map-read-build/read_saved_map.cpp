#include "saved_map.hpp"
extern "C" void norrath_map_read_saved_bytes(unsigned char* source) {
    using namespace norrath_saved_map_detail;
    const unsigned map = address(norrath_saved_map_tiles_bytes);
    unsigned input = address(source);
    for (unsigned row = 0; row != 108u; ++row) {
        const unsigned row_base = map + row * 864u;
        for (unsigned packed = 0; packed != 54u; ++packed) {
            const unsigned first = row_base + packed * 8u + 1u;
            const unsigned second = first + 4u;
            const unsigned high = unsigned(*bytes(input)) & 0xf0u;
            const unsigned low_first = unsigned(*bytes(first)) & 15u;
            const unsigned low_second = unsigned(*bytes(second)) & 15u;
            *bytes(first) = static_cast<unsigned char>(low_first | high);
            // Actual original byte is freshly read after the first map store.
            const unsigned low = unsigned(*bytes(input)) & 15u;
            ++input;
            *bytes(second) = static_cast<unsigned char>(low_second | (low << 4));
        }
    }
    for (unsigned row = 0; row != 108u; ++row) {
        const unsigned row_base = map + row * 864u;
        for (unsigned column = 0; column != 107u; ++column) {
            const unsigned previous = row_base + column * 4u + 1u;
            const unsigned next = previous + 4u;
            const unsigned target = previous + 2u;
            const unsigned left = unsigned(*bytes(previous)) >> 4;
            const unsigned right = unsigned(*bytes(next)) >> 4;
            const unsigned low = unsigned(*bytes(target)) & 15u;
            const unsigned averaged = (left + right + 1u) >> 1;
            *bytes(target) = static_cast<unsigned char>(low | (averaged << 4));
        }
        const unsigned edge = row_base + 431u;
        const unsigned low = unsigned(*bytes(edge)) & 15u;
        const unsigned high = unsigned(*bytes(edge - 2u)) & 0xf0u;
        *bytes(edge) = static_cast<unsigned char>(low | high);
    }
    for (unsigned column = 0; column != 216u; ++column) {
        const unsigned column_base = map + column * 2u;
        for (unsigned row = 0; row != 107u; ++row) {
            const unsigned previous = column_base + row * 864u + 1u;
            const unsigned next = previous + 864u;
            const unsigned target = previous + 432u;
            const unsigned above = unsigned(*bytes(previous)) >> 4;
            const unsigned below = unsigned(*bytes(next)) >> 4;
            const unsigned low = unsigned(*bytes(target)) & 15u;
            const unsigned averaged = (above + below + 1u) >> 1;
            *bytes(target) = static_cast<unsigned char>(low | (averaged << 4));
        }
        const unsigned edge = column_base + 92881u;
        const unsigned low = unsigned(*bytes(edge)) & 15u;
        const unsigned high = unsigned(*bytes(edge - 432u)) & 0xf0u;
        *bytes(edge) = static_cast<unsigned char>(low | high);
    }
}
