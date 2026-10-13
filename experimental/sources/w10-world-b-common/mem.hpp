#pragma once
// Little-endian field access for the world and game-loop sources of package w10-world-b (a copy of the same helper kept by w9-animation-object-a). Records are
// addressed as byte offsets because no original class declaration is claimed.
// A host test defines NORRATH_HOST_ARENA and supplies the two hooks below, since a
// 64-bit host cannot hold a 32-bit target pointer in a record word.
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(float) == 4,
              "target scalar representation");
static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "target byte order");
#ifndef NORRATH_HOST_ARENA
static_assert(sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4,
              "target32 pointer words");
#endif
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_address(const unsigned char* pointer);
#endif
namespace norrath_wb {
inline unsigned load_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) |
           (unsigned(p[3]) << 24);
}
inline void store_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
inline unsigned load_half(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8);
}
inline int load_signed_half(const unsigned char* p) {
    const unsigned value = load_half(p);
    return value < 0x8000u ? int(value) : int(value) - 0x10000;
}
inline void store_half(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
}
inline int load_signed_byte(const unsigned char* p) {
    return *p < 0x80u ? int(*p) : int(*p) - 0x100;
}
inline float load_float(const unsigned char* p) {
    const unsigned bits = load_word(p);
    float value;
    __builtin_memcpy(&value, &bits, sizeof value);
    return value;
}
inline void store_float(unsigned char* p, float value) {
    unsigned bits;
    __builtin_memcpy(&bits, &value, sizeof bits);
    store_word(p, bits);
}
inline unsigned char* load_pointer(const unsigned char* p) {
    const unsigned word = load_word(p);
#ifdef NORRATH_HOST_ARENA
    return word == 0 ? nullptr : norrath_host_pointer(word);
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
inline void store_pointer(unsigned char* p, const unsigned char* value) {
#ifdef NORRATH_HOST_ARENA
    store_word(p, value == nullptr ? 0u : norrath_host_address(value));
#else
    store_word(p, static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(value)));
#endif
}
}
