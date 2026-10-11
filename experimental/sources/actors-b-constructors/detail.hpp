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
// Existing run-list wrapper. Original objectAddToRunList__FP10GameObject.
unsigned norrath_actor_add_run_list(void* object);
// Original animAdd__FP14AnimationStateP15AnimationHeaderi. The published callee
// is void; this caller forwards the machine result, so the declaration returns it.
unsigned norrath_actor_animation_add(void* state, void* header, int flags);
// Original setPassableState__8GothTrapb at 0x225cc0. Not defined here.
unsigned norrath_goth_trap_set_passable_state(void* object, int enabled);
// Original setPassableState__10HateBridgeb at 0x225df0. Not defined here.
unsigned norrath_hate_bridge_set_passable_state(void* object, int enabled);
// Original worldGetDoorHeight__FP12_worldHeaderi at 0x17d3d0. Not defined here.
int norrath_world_get_door_height(void* world, int index);
// Observed vtables and the 4-byte `world` pointer at 0x4844a0. No payloads.
extern unsigned char norrath_actors_b_view_44ee08[];
extern unsigned char norrath_actors_b_view_44f0a8[];
extern unsigned char norrath_actors_b_view_44f150[];
extern unsigned char norrath_actors_b_view_44f000[];
extern unsigned char norrath_actors_b_view_44ef58[];
extern unsigned char norrath_actors_b_view_44eeb0[];
extern unsigned char norrath_actors_b_view_4844a0[];
unsigned norrath_construct_HackDoor(void* object, char* name, const void* point, short type, char** arguments);
unsigned norrath_construct_HateGate(void* object, char* name, const void* point, short type, char** arguments);
unsigned norrath_construct_HateInto(void* object, char* name, const void* point, short type, char** arguments);
unsigned norrath_construct_GothTrap(void* object, char* name, const void* point, short type, char** arguments);
unsigned norrath_construct_HateHole(void* object, char* name, const void* point, short type, char** arguments);
unsigned norrath_construct_HateBridge(void* object, char* name, const void* point, short type, char** arguments);
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
inline int signed_word(unsigned value) {
    return value <= 0x7fffffffu ? static_cast<int>(value) : -1 - static_cast<int>(0xffffffffu - value);
}
inline void write_float(Address a, float value) {
    const unsigned char* p = reinterpret_cast<const unsigned char*>(&value);
    write_word(a, unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24));
}
struct PointBytes { unsigned x, y, z; };
inline PointBytes snapshot_point(const void* point) {
    const Address a = address(point);
    return PointBytes{read_word(a), read_word(a + 4u), read_word(a + 8u)};
}
}
static_assert(sizeof(norrath_actors_b::PointBytes) == 12 && alignof(norrath_actors_b::PointBytes) == 4,
              "raw 12-byte point storage");
