#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4 &&
              sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4 && sizeof(float) == 4 &&
              __FLT_MANT_DIG__ == 24 && __FLT_MAX_EXP__ == 129 &&
              __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "target32 representation");
extern "C" {
// Factory ABI: object, point storage, and the integer kind in the third argument.
unsigned norrath_construct_Candle(void*, const void*, int);
unsigned norrath_game_object_construct(void*, const void*, void*, char**);
unsigned norrath_actor_add_run_list(void*);
// Retained lightAdd__FP5Light. The pointer is the light inside the object.
void norrath_light_add(void*);
extern unsigned char norrath_actors_a_view_44a9c0[];
extern unsigned char norrath_actors_a_view_481980[];
}
namespace norrath_actors_a {
using Address = unsigned;
inline Address address(const void* p) {
    return static_cast<Address>(reinterpret_cast<__UINTPTR_TYPE__>(p));
}
inline unsigned char* bytes(Address a) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(a));
}
inline void* pointer(Address a) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(a));
}
inline unsigned read_word(Address a) {
    const unsigned char* p = bytes(a);
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
inline void write_word(Address a, unsigned v) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(v);
    p[1] = static_cast<unsigned char>(v >> 8);
    p[2] = static_cast<unsigned char>(v >> 16);
    p[3] = static_cast<unsigned char>(v >> 24);
}
inline void write_half(Address a, unsigned v) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(v);
    p[1] = static_cast<unsigned char>(v >> 8);
}
}
