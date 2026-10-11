#include "memory_compare.hpp"

extern "C" int norrath_memory_compare(const void* left, const void* right,
                                       __SIZE_TYPE__ count) {
    const unsigned char* left_bytes = static_cast<const unsigned char*>(left);
    const unsigned char* right_bytes = static_cast<const unsigned char*>(right);
    for (__SIZE_TYPE__ offset = 0; offset < count; ++offset) {
        const unsigned left_byte = left_bytes[offset];
        const unsigned right_byte = right_bytes[offset];
        if (left_byte != right_byte)
            return static_cast<int>(left_byte) - static_cast<int>(right_byte);
    }
    return 0;
}
