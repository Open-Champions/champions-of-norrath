#pragma once
// Little-endian field helpers for the save-and-quests bodies. Object fields stay
// at their observed byte offsets. Host checks define NORRATH_SQ_HOST because a
// host pointer does not fit in a 32-bit object word.
#if !defined(NORRATH_SQ_HOST)
static_assert(sizeof(unsigned) == 4 && sizeof(int) == 4 && sizeof(short) == 2 &&
              sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4 &&
              sizeof(float) == 4 && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__,
              "target32 representation");
#endif
namespace sq {
using Address = __UINTPTR_TYPE__;
inline Address address(const void* p) { return reinterpret_cast<Address>(p); }
inline unsigned char* bytes(Address a) { return reinterpret_cast<unsigned char*>(a); }
#if defined(NORRATH_SQ_HOST)
struct HostPointer { unsigned bits; void* full; };
inline HostPointer host_rows[64] = {};
inline void host_note(void* full) {
    const unsigned bits = static_cast<unsigned>(reinterpret_cast<Address>(full));
    for (unsigned index = 0u; index < 64u; ++index) {
        if (host_rows[index].full == nullptr || host_rows[index].bits == bits) {
            host_rows[index].bits = bits;
            host_rows[index].full = full;
            return;
        }
    }
}
inline void* pointer(unsigned bits) {
    for (unsigned index = 0u; index < 64u; ++index) {
        if (host_rows[index].full != nullptr && host_rows[index].bits == bits) return host_rows[index].full;
    }
    return reinterpret_cast<void*>(static_cast<Address>(bits));
}
inline unsigned pointer_bits(const void* p) {
    host_note(const_cast<void*>(p));
    return static_cast<unsigned>(address(p));
}
#else
inline void* pointer(unsigned bits) { return reinterpret_cast<void*>(bits); }
inline unsigned pointer_bits(const void* p) { return static_cast<unsigned>(address(p)); }
#endif
inline unsigned read_word(const void* a) {
    const unsigned char* p = static_cast<const unsigned char*>(a);
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
inline void write_word(void* a, unsigned value) {
    unsigned char* p = static_cast<unsigned char*>(a);
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
inline unsigned long long read_u64(const void* a) {
    const unsigned char* p = static_cast<const unsigned char*>(a);
    return static_cast<unsigned long long>(read_word(p)) | (static_cast<unsigned long long>(read_word(p + 4)) << 32);
}
inline float read_single(const void* a) { return __builtin_bit_cast(float, read_word(a)); }
inline void write_single(void* a, float value) { write_word(a, __builtin_bit_cast(unsigned, value)); }
inline int signed_word(unsigned value) {
    return value <= 0x7fffffffu ? static_cast<int>(value) : -1 - static_cast<int>(0xffffffffu - value);
}
}
