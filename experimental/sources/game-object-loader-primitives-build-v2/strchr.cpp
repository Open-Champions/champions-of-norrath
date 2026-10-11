#include "string_primitives.hpp"
extern "C" char* norrath_factory_strchr(const char* text, int character) {
    using namespace norrath_string_detail;
    Address cursor = address(text);
    const unsigned wanted = static_cast<unsigned>(character) & 255u;
    Word64 repeated = static_cast<Word64>(wanted);
    repeated |= repeated << 8;
    repeated |= repeated << 16;
    repeated |= repeated << 32;
    if ((cursor & 7u) == 0u) {
        if ((cursor & 15u) == 0u) {
            for (;;) {
                const Block16 block = read16(cursor);
                if (contains_zero(block)) break;
                const Block16 different{block.low ^ repeated, block.high ^ repeated};
                if (contains_zero(different)) break;
                cursor += 16u;
            }
        } else {
            for (;;) {
                const Word64 block = read8(cursor);
                if (contains_zero(block)) break;
                if (contains_zero(block ^ repeated)) break;
                cursor += 8u;
            }
        }
    }
    for (;;) {
        const unsigned value = read_byte(cursor);
        if (value == wanted)
            return reinterpret_cast<char*>(static_cast<__UINTPTR_TYPE__>(cursor));
        if (value == 0u) return nullptr;
        cursor += 1u;
    }
}
