#pragma once
// Shared byte helpers for the lava-monster-through-wraith constructors.
// Addresses stored in object fields are the low 32 bits. Host checks define
// NORRATH_CTORS_LAVA_WRAITH_HOST so the same statements can run where pointers
// are wider than 32 bits.
#if !defined(NORRATH_CTORS_LAVA_WRAITH_HOST)
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4 && sizeof(short) == 2 &&
              sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4 && sizeof(unsigned long long) == 8 &&
              sizeof(float) == 4 && __FLT_MANT_DIG__ == 24 && __FLT_MAX_EXP__ == 129 &&
              __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "target32 representation");
#else
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(float) == 4 &&
              __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "host scalar representation");
#endif
extern "C" {
unsigned norrath_game_object_construct(void* object, const void* point, void* resource_map, char** arguments);
char* norrath_actor_find_tag_string(void* object, char* tag, int occurrence);
int norrath_world_find_closest_door(void* world, const void* point);
int norrath_world_set_door_collision(void* world, int index, int value);
int norrath_world_get_door_height(void* world, int index);
unsigned norrath_object_add_to_delete_list(void* object);
extern unsigned char norrath_actors_a_view_44d0a8[];
extern unsigned char norrath_actors_a_view_485358[];
extern unsigned char norrath_factory_world_pointer_word[];
extern unsigned char norrath_ctors_lava_wraith_view_449820[];
void norrath_light_add(void* light);
extern unsigned char norrath_ctors_lava_wraith_view_44ab70[];
unsigned norrath_construct_Torch(void* object, const void* point, char** arguments, int kind);
unsigned norrath_construct_Switch(void* object, const void* point, char** arguments);
unsigned norrath_construct_RollDoor(void* object, const void* point, short type, char** arguments);
extern unsigned char norrath_ctors_lava_wraith_view_42c630[];
extern unsigned char norrath_ctors_lava_wraith_view_449940[];
}
namespace norrath_ctors_lava_wraith {
using Address = __UINTPTR_TYPE__;
using Wide = unsigned long long;
inline Address address(const void* p) { return reinterpret_cast<Address>(p); }
inline unsigned address_bits(const void* p) { return static_cast<unsigned>(address(p)); }
inline unsigned char* bytes(Address a) { return reinterpret_cast<unsigned char*>(a); }
#if defined(NORRATH_CTORS_LAVA_WRAITH_HOST)
struct HostPointer { unsigned bits; void* full; };
inline HostPointer host_rows[64] = {};
inline void host_note(const void* full) {
    const unsigned bits = static_cast<unsigned>(reinterpret_cast<Address>(full));
    for (unsigned index = 0u; index < 64u; ++index) {
        if (host_rows[index].full == nullptr || host_rows[index].bits == bits) {
            host_rows[index].bits = bits;
            host_rows[index].full = const_cast<void*>(full);
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
inline unsigned read_byte(Address a) { return bytes(a)[0]; }
inline void write_byte(Address a, unsigned value) { bytes(a)[0] = static_cast<unsigned char>(value); }
inline unsigned float_bits(float value) { return __builtin_bit_cast(unsigned, value); }
inline float bits_float(unsigned value) { return __builtin_bit_cast(float, value); }
inline void write_half(Address a, unsigned value) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
}
inline int read_signed_half(Address a) {
    const unsigned char* p = bytes(a);
    const unsigned value = unsigned(p[0]) | (unsigned(p[1]) << 8);
    return value < 0x8000u ? static_cast<int>(value) : static_cast<int>(value) - 0x10000;
}
inline int low_signed(int value) { return static_cast<int>(static_cast<short>(value)); }
inline void store_pointer(Address a, const void* p) {
#if defined(NORRATH_CTORS_LAVA_WRAITH_HOST)
    host_note(p);
#endif
    write_word(a, address_bits(p));
}
inline void* load_pointer(Address a) { return pointer(read_word(a)); }
inline void* world_pointer() { return load_pointer(address(norrath_factory_world_pointer_word)); }
struct PointBytes { unsigned x, y, z; };
inline PointBytes snapshot_point(const void* point) {
    const Address a = address(point);
    return PointBytes{read_word(a), read_word(a + 4u), read_word(a + 8u)};
}
inline void write_point(Address a, PointBytes point) {
    write_word(a, point.x);
    write_word(a + 4u, point.y);
    write_word(a + 8u, point.z);
}
}
static_assert(sizeof(norrath_ctors_lava_wraith::PointBytes) == 12 &&
              alignof(norrath_ctors_lava_wraith::PointBytes) == 4, "raw 12-byte point storage");
