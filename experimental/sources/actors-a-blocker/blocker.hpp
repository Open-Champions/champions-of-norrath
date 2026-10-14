#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4 &&
              sizeof(short) == 2 && sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4 &&
              sizeof(float) == 4 && __FLT_MANT_DIG__ == 24 && __FLT_MAX_EXP__ == 129 &&
              __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "target32 representation");
extern "C" {
// Factory ABI: object, 12-byte point storage, signed16 type, argument list.
unsigned norrath_construct_Blocker(void*, const void*, short, char**);
// Existing complete object and tag providers. Not re-authored here.
unsigned norrath_game_object_construct(void*, const void*, void*, char**);
char* norrath_actor_find_tag_string(void*, char*, int);
int norrath_game_object_find_tag_int(void*, char*, int);
// Address identities for the Blocker view and the three tag words it reads.
extern unsigned char norrath_actors_a_view_44c830[];
extern unsigned char norrath_actors_a_view_485400[];
extern unsigned char norrath_actors_a_view_485408[];
extern unsigned char norrath_actors_a_view_470830[];
}
namespace norrath_actors_a {
using Address = unsigned;
inline Address address(const void* p) {
    return static_cast<Address>(reinterpret_cast<__UINTPTR_TYPE__>(p));
}
inline unsigned char* bytes(Address a) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(a));
}
inline char* text(Address a) { return reinterpret_cast<char*>(bytes(a)); }
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
