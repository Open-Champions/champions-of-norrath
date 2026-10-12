#pragma once
// Little-endian byte helpers for the Gold and ItemProp object methods. Object
// words are 32 bits; the host checks only call these on host buffers.
namespace inventory_object {
inline unsigned low_pointer(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
inline unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
inline void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
inline void copy_bytes(unsigned char* destination, const unsigned char* source, unsigned count) {
    for (unsigned i = 0; i < count; ++i) destination[i] = source[i];
}
}
