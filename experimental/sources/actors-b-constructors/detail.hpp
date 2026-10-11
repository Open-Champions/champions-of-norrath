#pragma once
// Shared byte-view helpers for G–Z actor constructors. Addresses stored into
// object fields are the low 32 bits. Host checks define NORRATH_ACTORS_B_HOST
// so the same statements can run where pointers are wider than 32 bits.
#if !defined(NORRATH_ACTORS_B_HOST)
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4 && sizeof(short) == 2 &&
              sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4 && sizeof(unsigned long long) == 8 &&
              sizeof(float) == 4 && __FLT_MANT_DIG__ == 24 && __FLT_MAX_EXP__ == 129 &&
              __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "target32 representation");
#else
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(float) == 4 &&
              __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "host scalar representation");
#endif
extern "C" {
// Actors-a parent. Original __8AutoPropPcG6Point3sPPc. Not defined here.
unsigned norrath_construct_AutoProp(void* object, char* name, const void* point, short type, char** arguments);
// Observed vtable _vt$8HackDoor at 0x44ee08. Address identity only; no payload.
extern unsigned char norrath_actors_b_view_44ee08[];
unsigned norrath_construct_HackDoor(void* object, char* name, const void* point, short type, char** arguments);
}
namespace norrath_actors_b {
using Address = __UINTPTR_TYPE__;
inline Address address(const void* p) { return reinterpret_cast<Address>(p); }
inline unsigned address_bits(const void* p) { return static_cast<unsigned>(address(p)); }
inline unsigned char* bytes(Address a) { return reinterpret_cast<unsigned char*>(a); }
inline void* pointer(Address a) { return reinterpret_cast<void*>(a); }
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
struct PointBytes { unsigned x, y, z; };
inline PointBytes snapshot_point(const void* point) {
    const Address a = address(point);
    return PointBytes{read_word(a), read_word(a + 4u), read_word(a + 8u)};
}
}
static_assert(sizeof(norrath_actors_b::PointBytes) == 12 && alignof(norrath_actors_b::PointBytes) == 4,
              "raw 12-byte point storage");
