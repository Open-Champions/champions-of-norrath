#pragma once
// Little-endian field helpers for virtual-slot bodies. Object fields stay at
// their observed byte offsets. Host checks define NORRATH_VSLOTS_B_HOST because
// a host pointer does not fit in a 32-bit object word.
#if !defined(NORRATH_VSLOTS_B_HOST)
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4 && sizeof(short) == 2 &&
              sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4 && sizeof(unsigned long long) == 8 &&
              sizeof(float) == 4 && __FLT_MANT_DIG__ == 24 && __FLT_MAX_EXP__ == 129 &&
              __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "target32 representation");
#else
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(float) == 4 &&
              __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "host scalar representation");
#endif
namespace vslots_b {
using Address = __UINTPTR_TYPE__;
inline Address address(const void* p) { return reinterpret_cast<Address>(p); }
inline unsigned address_bits(const void* p) { return static_cast<unsigned>(address(p)); }
inline unsigned char* bytes(Address a) { return reinterpret_cast<unsigned char*>(a); }
#if defined(NORRATH_VSLOTS_B_HOST)
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
inline void* pointer(Address a) {
    const unsigned bits = static_cast<unsigned>(a);
    for (unsigned index = 0u; index < 64u; ++index) {
        if (host_rows[index].full != nullptr && host_rows[index].bits == bits) return host_rows[index].full;
    }
    return reinterpret_cast<void*>(a);
}
#else
inline void* pointer(Address a) { return reinterpret_cast<void*>(a); }
#endif
inline unsigned read_word(Address a) {
    const unsigned char* p = bytes(a);
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
inline void write_word(Address a, unsigned value) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
inline unsigned read_u16(Address a) {
    const unsigned char* p = bytes(a);
    return unsigned(p[0]) | (unsigned(p[1]) << 8);
}
inline short read_i16(Address a) {
    const unsigned value = read_u16(a);
    return value < 0x8000u ? static_cast<short>(value) : static_cast<short>(static_cast<int>(value) - 65536);
}
inline void write_u16(Address a, unsigned value) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
}
inline unsigned read_u8(Address a) { return bytes(a)[0]; }
inline void write_u8(Address a, unsigned value) { bytes(a)[0] = static_cast<unsigned char>(value); }
inline unsigned long long read_u64(Address a) {
    unsigned long long value = 0;
    for (unsigned i = 0; i < 8u; ++i) value |= static_cast<unsigned long long>(bytes(a)[i]) << (8u * i);
    return value;
}
inline void write_u64(Address a, unsigned long long value) {
    for (unsigned i = 0; i < 8u; ++i) bytes(a)[i] = static_cast<unsigned char>(value >> (8u * i));
}
__attribute__((always_inline)) inline float read_single(Address a) {
    const unsigned word = read_word(a);
    float value;
    unsigned char* out = reinterpret_cast<unsigned char*>(&value);
    out[0] = static_cast<unsigned char>(word);
    out[1] = static_cast<unsigned char>(word >> 8);
    out[2] = static_cast<unsigned char>(word >> 16);
    out[3] = static_cast<unsigned char>(word >> 24);
    return value;
}
__attribute__((always_inline)) inline void write_single(Address a, float value) {
    const unsigned char* in = reinterpret_cast<const unsigned char*>(&value);
    write_word(a, unsigned(in[0]) | (unsigned(in[1]) << 8) | (unsigned(in[2]) << 16) | (unsigned(in[3]) << 24));
}
inline int signed_word(unsigned value) {
    return value <= 0x7fffffffu ? static_cast<int>(value) : -1 - static_cast<int>(0xffffffffu - value);
}
// Observed virtual calls load a signed halfword adjustment and the function
// pointer four bytes later. Slot offsets are byte offsets from the vtable.
template <typename Function>
Function slot_function(void* self, unsigned slot_byte_offset, void** adjusted_self) {
    const unsigned table_bits = read_word(address(self) + 72u);
    const Address slot = address(pointer(table_bits)) + slot_byte_offset;
    const short adjustment = read_i16(slot);
    *adjusted_self = static_cast<unsigned char*>(self) + adjustment;
    return reinterpret_cast<Function>(pointer(read_word(slot + 4u)));
}
}
