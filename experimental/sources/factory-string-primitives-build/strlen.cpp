#include "string_primitives.hpp"
extern "C" unsigned norrath_factory_strlen(const char* text) {
    using namespace norrath_string_detail;
    const Address start = address(text);
    Address cursor = start;
    if ((cursor & 7u) == 0u) {
        if ((cursor & 15u) == 0u) {
            while (!contains_zero(read16(cursor))) cursor += 16u;
        } else {
            while (!contains_zero(read8(cursor))) cursor += 8u;
        }
    }
    // Misaligned input remains scalar. A detected aligned block is scanned from
    // its beginning, rather than using mask bit positions as a byte index.
    while (read_byte(cursor) != 0u) cursor += 1u;
    return cursor - start;
}
