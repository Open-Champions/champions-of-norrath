#include "move_bytes.hpp"
namespace norrath_archive_move_detail {
using namespace norrath_string_detail;
inline volatile unsigned char* writable(Address a) {
    return reinterpret_cast<volatile unsigned char*>(static_cast<__UINTPTR_TYPE__>(a));
}
inline void write8(Address a, Word64 value) {
    for (unsigned byte = 0; byte < 8u; ++byte)
        writable(a)[byte] = static_cast<unsigned char>(value >> (8u * byte));
}
inline void write16(Address a, Block16 value) {
    write8(a, value.low);
    write8(a + 8u, value.high);
}
}
extern "C" void* norrath_archive_move_bytes(void* destination, const void* source, unsigned count) {
    using namespace norrath_archive_move_detail;
    Address output = address(destination), input = address(source);
    const Address source_end = input + count; // Actual low32 wrapped sum.
    if (input < output && output < source_end) {
        output += count;
        input = source_end;
        // The actual right-overlap path uses descending scalar bytes only,
        // regardless of alignment or count. No aligned backward substitute.
        while (count != 0u) {
            input -= 1u;
            output -= 1u;
            const unsigned char value = read_byte(input);
            *writable(output) = value;
            count -= 1u;
        }
        return destination;
    }
    if (count >= 32u && ((output | input) & 15u) == 0u) {
        do {
            // Actual full first16 capture and store precedes second16 capture.
            // Preserve this order even for equal or left-overlapping pointers.
            const Block16 first = read16(input);
            write16(output, first);
            input += 16u;
            output += 16u;
            const Block16 second = read16(input);
            write16(output, second);
            input += 16u;
            output += 16u;
            count -= 32u;
        } while (count >= 32u);
        // Original LD/SD tail is reached only after at least one aligned32 block.
        while (count >= 8u) {
            const Word64 block = read8(input);
            write8(output, block);
            input += 8u;
            output += 8u;
            count -= 8u;
        }
    }
    while (count != 0u) {
        const unsigned char value = read_byte(input);
        *writable(output) = value;
        input += 1u;
        output += 1u;
        count -= 1u;
    }
    return destination;
}
