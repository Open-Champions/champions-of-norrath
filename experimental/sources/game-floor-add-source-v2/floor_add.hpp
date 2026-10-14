#pragma once
#include "ee_compare.hpp"
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4 &&
              sizeof(float) == 4 && sizeof(void*) == 4 &&
              sizeof(__UINTPTR_TYPE__) == 4 && sizeof(long long) == 8 &&
              __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "EE word/float/address model");
extern "C" {
// Exact unsized views shared with floorClear; no owner or binding is defined here.
extern unsigned char norrath_floor_sprite_grid_view[];
extern unsigned char norrath_floor_free_list_word_view[];
extern unsigned char norrath_floor_sprite_count_word_view[];
extern unsigned char norrath_floor_expand_list_view[];
extern unsigned char norrath_floor_blood_level_word_view[];
void norrath_floor_add(const unsigned char* point, int type_word, float extent,
                      int kind, int flags);
// Genuine provider interfaces. Lighting and trig admission/integration are separate.
unsigned norrath_light_floor_sprite(const unsigned char* point);
float norrath_integer_cosine(int angle);
float norrath_integer_sine(int angle);
// Existing authored EE architectural primitive: execute CVT.W.S and return its bits.
unsigned ee_cvt_w_s_word(float value);
}
namespace norrath_floor_add_detail {
using Address = unsigned;
// One target operation per template; operand positions are intentionally fixed.
// The memory clobber prevents surrounding memory accesses crossing the operation.
inline float ordered_add(float left, float right) {
    float result;
    __asm__ volatile ("add.s %0, %1, %2" : "=f" (result)
                      : "f" (left), "f" (right) : "memory");
    return result;
}
inline float ordered_multiply(float left, float right) {
    float result;
    __asm__ volatile ("mul.s %0, %1, %2" : "=f" (result)
                      : "f" (left), "f" (right) : "memory");
    return result;
}
inline Address address(const void* p) {
    return static_cast<Address>(reinterpret_cast<__UINTPTR_TYPE__>(p));
}
inline volatile unsigned char* bytes(Address a) {
    return reinterpret_cast<volatile unsigned char*>(static_cast<__UINTPTR_TYPE__>(a));
}
inline unsigned read_word(Address a) {
    const unsigned b0 = bytes(a)[0];
    const unsigned b1 = bytes(a)[1];
    const unsigned b2 = bytes(a)[2];
    const unsigned b3 = bytes(a)[3];
    return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}
inline unsigned read_half(Address a) {
    const unsigned b0 = bytes(a)[0];
    const unsigned b1 = bytes(a)[1];
    return b0 | (b1 << 8);
}
inline void write_word(Address a, unsigned v) {
    bytes(a)[0] = static_cast<unsigned char>(v);
    bytes(a)[1] = static_cast<unsigned char>(v >> 8);
    bytes(a)[2] = static_cast<unsigned char>(v >> 16);
    bytes(a)[3] = static_cast<unsigned char>(v >> 24);
}
inline void write_half(Address a, unsigned v) {
    bytes(a)[0] = static_cast<unsigned char>(v);
    bytes(a)[1] = static_cast<unsigned char>(v >> 8);
}
inline int signed_half(unsigned v) {
    v &= 0xffffu;
    return v < 0x8000u ? static_cast<int>(v) : static_cast<int>(v) - 65536;
}
inline long long signed_word(unsigned v) {
    return v < 0x80000000u ? static_cast<long long>(v)
                         : static_cast<long long>(v) - 0x100000000LL;
}
inline unsigned shift_right_signed_eight(unsigned v) {
    return (v >> 8) | ((0u - (v >> 31)) << 24);
}
inline float float_word(unsigned word) {
    float value;
    unsigned char* p = reinterpret_cast<unsigned char*>(&value);
    p[0] = static_cast<unsigned char>(word);
    p[1] = static_cast<unsigned char>(word >> 8);
    p[2] = static_cast<unsigned char>(word >> 16);
    p[3] = static_cast<unsigned char>(word >> 24);
    return value;
}
inline float read_float(Address a) { return float_word(read_word(a)); }
inline void write_float(Address a, float value) {
    const unsigned char* p = reinterpret_cast<const unsigned char*>(&value);
    bytes(a)[0] = p[0]; bytes(a)[1] = p[1];
    bytes(a)[2] = p[2]; bytes(a)[3] = p[3];
}
}
