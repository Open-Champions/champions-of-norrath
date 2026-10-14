#pragma once
#include "block_alloc.hpp"
namespace norrath_block_view {
inline unsigned address(const void* p) { return unsigned(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
inline unsigned char* bytes(unsigned a) { return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(a)); }
inline unsigned read_word(unsigned a) {
    const unsigned char* p = bytes(a);
    return unsigned(p[0]) | unsigned(p[1]) << 8 | unsigned(p[2]) << 16 | unsigned(p[3]) << 24;
}
inline void write_word(unsigned a, unsigned value) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
inline int signed_word(unsigned word) {
    return word <= 0x7fffffffu ? int(word) : -1 - int(0xffffffffu - word);
}
}
