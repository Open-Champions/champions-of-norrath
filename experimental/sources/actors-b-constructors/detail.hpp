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
// Existing parents and list wrappers. Not defined here.
unsigned norrath_game_object_construct(void* object, const void* point, void* resource_map, char** arguments);
unsigned norrath_creature_construct(void* object, const void* point, short type, void* resource_map, float scale, char** arguments);
unsigned norrath_actor_add_slow_run_list(void* object);
// Original objectAddToMediumRunList__FP10GameObject at 0x143908.
unsigned norrath_actor_add_medium_run_list(void* object);
int norrath_game_object_find_tag_int(void* object, char* tag, int occurrence);
char* norrath_actor_find_tag_string(void* object, char* tag, int occurrence);
// Original __7VehicleG6Point3sPPci at 0x2702d8 and Reset__7Vehiclei at 0x270f28.
unsigned norrath_construct_Vehicle(void* object, const void* point, short type, char** arguments, int flag);
unsigned norrath_vehicle_reset(void* object, int value);
// Original worldCheckForWater__FP12_worldHeaderG6Point3fPf at 0x17db78.
int norrath_world_check_for_water(void* world, const void* point, float radius, float* output);
extern unsigned char norrath_actors_b_view_448e30[];
extern unsigned char norrath_actors_b_view_448a70[];
extern unsigned char norrath_actors_b_view_44a4b0[];
extern unsigned char norrath_actors_b_view_44a420[];
extern unsigned char norrath_actors_b_view_44c8c0[];
extern unsigned char norrath_actors_b_view_450250[];
extern unsigned char norrath_actors_b_view_453230[];
extern unsigned char norrath_actors_b_view_4816d4[];
extern unsigned char norrath_actors_b_view_4850f0[];
extern unsigned char norrath_actors_b_view_4850f8[];
extern unsigned char norrath_actors_b_view_484fd8[];
extern unsigned char norrath_actors_b_view_484fe0[];
extern unsigned char norrath_actors_b_view_484fe8[];
unsigned norrath_construct_Teleporter(void* object, const void* point, short type, char** arguments);
unsigned norrath_construct_PushPhysicsPropSimple(void* object, const void* point);
unsigned norrath_construct_MissileTrap(void* object, const void* point, int type, char** arguments);
unsigned norrath_construct_Shooter(void* object, const void* point, int type, char** arguments);
unsigned norrath_construct_Trigger(void* object, const void* point, char** arguments);
unsigned norrath_construct_LavaTractor(void* object, const void* point, short type, char** arguments);
unsigned norrath_construct_SkulBoat(void* object, const void* point, short type, char** arguments);
}
namespace norrath_actors_b {
using Address = __UINTPTR_TYPE__;
inline Address address(const void* p) { return reinterpret_cast<Address>(p); }
inline unsigned address_bits(const void* p) { return static_cast<unsigned>(address(p)); }
inline unsigned char* bytes(Address a) { return reinterpret_cast<unsigned char*>(a); }
#if defined(NORRATH_ACTORS_B_HOST)
// Host pointers do not fit in the 32-bit fields this code reloads. Tests record
// the full pointer; EE builds keep the direct cast below.
struct HostPointer { unsigned bits; void* full; };
inline HostPointer norrath_actors_b_host_rows[64] = {};
inline void host_note(void* full) {
    const unsigned bits = static_cast<unsigned>(reinterpret_cast<Address>(full));
    for (unsigned index = 0u; index < 64u; ++index) {
        if (norrath_actors_b_host_rows[index].full == nullptr || norrath_actors_b_host_rows[index].bits == bits) {
            norrath_actors_b_host_rows[index].bits = bits;
            norrath_actors_b_host_rows[index].full = full;
            return;
        }
    }
}
inline void* pointer(Address a) {
    const unsigned bits = static_cast<unsigned>(a);
    for (unsigned index = 0u; index < 64u; ++index) {
        if (norrath_actors_b_host_rows[index].full != nullptr && norrath_actors_b_host_rows[index].bits == bits)
            return norrath_actors_b_host_rows[index].full;
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
inline int signed_word(unsigned value) {
    return value <= 0x7fffffffu ? static_cast<int>(value) : -1 - static_cast<int>(0xffffffffu - value);
}
inline void write_float(Address a, float value) {
    const unsigned char* p = reinterpret_cast<const unsigned char*>(&value);
    write_word(a, unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24));
}
inline float read_float(Address a) {
    const unsigned word = read_word(a);
    float value;
    unsigned char* p = reinterpret_cast<unsigned char*>(&value);
    p[0] = static_cast<unsigned char>(word);
    p[1] = static_cast<unsigned char>(word >> 8);
    p[2] = static_cast<unsigned char>(word >> 16);
    p[3] = static_cast<unsigned char>(word >> 24);
    return value;
}
inline void write_half(Address a, unsigned value) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
}
// Finite singles only. Original min.s/max.s NaN and signed-zero results are unknown.
inline float minimum_single(float left, float right) { return right < left ? right : left; }
inline float maximum_single(float left, float right) { return left < right ? right : left; }
struct PointBytes { unsigned x, y, z; };
inline PointBytes snapshot_point(const void* point) {
    const Address a = address(point);
    return PointBytes{read_word(a), read_word(a + 4u), read_word(a + 8u)};
}
}
static_assert(sizeof(norrath_actors_b::PointBytes) == 12 && alignof(norrath_actors_b::PointBytes) == 4,
              "raw 12-byte point storage");
