#include "string_primitives.hpp"
namespace {
using namespace norrath_string_detail;
void write8(Address at, Word64 value) {
    unsigned char* output = reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(at));
    for (unsigned byte = 0; byte < 8u; ++byte)
        output[byte] = static_cast<unsigned char>(value >> (8u * byte));
}
void write16(Address at, Block16 value) {
    write8(at, value.low);
    write8(at + 8u, value.high);
}
}
extern "C" char* norrath_factory_strcpy(char* destination_text, const char* source_text) {
    using namespace norrath_string_detail;
    const Address start = address(destination_text);
    Address destination = start;
    Address source = address(source_text);
    const Address combined = destination | source;
    if ((combined & 7u) == 0u) {
        if ((combined & 15u) == 0u) {
            for (;;) {
                const Block16 block = read16(source);
                if (contains_zero(block)) break;
                write16(destination, block);
                source += 16u;
                destination += 16u;
            }
        } else {
            for (;;) {
                const Word64 block = read8(source);
                if (contains_zero(block)) break;
                write8(destination, block);
                source += 8u;
                destination += 8u;
            }
        }
    }
    for (;;) {
        const unsigned char value = read_byte(source);
        source += 1u;
        unsigned char* output = reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(destination));
        *output = value;
        destination += 1u;
        if (value == 0u) return destination_text;
    }
}
