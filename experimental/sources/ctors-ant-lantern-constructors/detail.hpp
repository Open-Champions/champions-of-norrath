#pragma once
// Byte helpers for the Ant-through-Lantern constructors. Stored addresses are
// the low 32 bits. Host checks define NORRATH_CTORS_ANT_LANTERN_HOST.
#if !defined(NORRATH_CTORS_ANT_LANTERN_HOST)
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4 && sizeof(short) == 2 &&
              sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4 && sizeof(unsigned long long) == 8 &&
              sizeof(float) == 4 && __FLT_MANT_DIG__ == 24 && __FLT_MAX_EXP__ == 129 &&
              __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "target32 representation");
#else
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(float) == 4 &&
              __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "host scalar representation");
#endif
extern "C" {
// Original blockAlloc__Fi. Already defined outside this package.
unsigned char* norrath_block_alloc(int requested_size);
// Original __11AddParticleG6Point3PPc. Not defined here.
unsigned __11AddParticleG6Point3PPc(void* object, const void* point, char** arguments);
// Parents defined later in this package, or left unresolved until then.
unsigned norrath_construct_AutoChest(void* object, char* name, const void* point, short type, char** arguments);
unsigned norrath_construct_AutoContainer(void* object, char* name, const void* point, short type, char** arguments);
unsigned norrath_construct_AutoChestLP(void* object, char* name, const void* point, short type, char** arguments);
unsigned norrath_construct_AutoContainerLP(void* object, char* name, const void* point, short type, char** arguments);
unsigned norrath_construct_Boat(void* object, const void* point, short type, char** arguments);
// Original __7VehicleG6Point3sPPci and Reset__7Vehiclei. Defined by vehicle-actors.
unsigned norrath_construct_Vehicle(void* object, const void* point, short type, char** arguments, int flag);
unsigned norrath_vehicle_reset(void* object, int value);
// Original worldCheckForWater__FP12_worldHeaderG6Point3fPf. Not defined here.
int norrath_world_check_for_water(void* world, const void* point, float radius, float* output);
// Existing 4-byte `world` pointer at 0x4844a0.
extern unsigned char norrath_actors_b_view_4844a0[];
extern unsigned char norrath_ctors_ant_lantern_view_44e940[];
extern unsigned char norrath_ctors_ant_lantern_view_453328[];
// Vtable objects. Slots stay unresolved.
extern unsigned char norrath_ctors_ant_lantern_view_44ea78[];
}
namespace norrath_ctors_ant_lantern {
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
static_assert(sizeof(norrath_ctors_ant_lantern::PointBytes) == 12 &&
              alignof(norrath_ctors_ant_lantern::PointBytes) == 4, "raw 12-byte point storage");
