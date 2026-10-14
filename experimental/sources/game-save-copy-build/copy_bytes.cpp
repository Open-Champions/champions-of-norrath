#include "string_primitives.hpp"
static_assert(sizeof(__SIZE_TYPE__) == 4, "original memcpy count32 transport");
namespace norrath_save_copy_detail {
using namespace norrath_string_detail;
inline volatile unsigned char* writable(Address a) {
    return reinterpret_cast<volatile unsigned char*>(static_cast<__UINTPTR_TYPE__>(a));
}
inline void write8(Address a, Word64 value) {
    for (unsigned byte=0; byte<8u; ++byte)
        writable(a)[byte] = static_cast<unsigned char>(value >> (8u*byte));
}
inline void write16(Address a, Block16 value) {
    write8(a, value.low);
    write8(a + 8u, value.high);
}
}
extern "C" void* norrath_copy_bytes(void* destination, const void* source, __SIZE_TYPE__ size) {
    using namespace norrath_save_copy_detail;
    Address output = address(destination);
    Address input = address(source);
    unsigned count = static_cast<unsigned>(size);
    if (count >= 32u && ((output | input) & 15u) == 0u) {
        do {
            // Preserve actual LQ/SQ ordering, including lawful overlapping RAM:
            // first16 fully captured/stored BEFORE the second16 is captured.
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
        // The original eight-byte path is reached only after the aligned32 path.
        while (count >= 8u) {
            const Word64 block = read8(input);
            write8(output, block);
            count -= 8u;
            input += 8u;
            output += 8u;
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
