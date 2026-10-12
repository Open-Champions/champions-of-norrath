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
void* norrath_lump_find_resource(char* lump_name, char* resource_name);
void norrath_body_init(void* body, float a, float b, float c, float d, float e);
unsigned norrath_actor_add_run_list(void* object);
extern unsigned char norrath_actors_b_view_448a70[];
unsigned norrath_construct_PushPhysicsProp(void* object, const void* point, short type, char* lump, char* first, char* second, float scale);
int norrath_game_object_find_tag_int(void* object, char* tag, int occurrence);
int strcmp(const char* left, const char* right);
// Original addIcon__3mapP6Point3iPciT3N33 at 0x15c320. Not defined here.
unsigned norrath_map_add_icon(const void* point, int kind, char* name, int type, char* first, char* second, char* third, char* fourth);
extern unsigned char norrath_ctors_lava_wraith_view_44c710[];
extern unsigned char norrath_ctors_lava_wraith_view_46cd20[];
extern unsigned char norrath_ctors_lava_wraith_view_484fa0[];
extern unsigned char norrath_ctors_lava_wraith_view_484fa8[];
extern unsigned char norrath_ctors_lava_wraith_view_46cd30[];
extern unsigned char norrath_ctors_lava_wraith_view_46cd40[];
extern unsigned char norrath_ctors_lava_wraith_view_46cd50[];
extern unsigned char norrath_ctors_lava_wraith_view_46cd60[];
extern unsigned char norrath_ctors_lava_wraith_view_46cd70[];
unsigned norrath_construct_PushTrigger(void* object, const void* point, int type, char** arguments);
int norrath_game_object_e_rand(void* object);
float norrath_player_treasure_ramp();
float norrath_game_object_find_tag_float(void* object, char* tag, int occurrence);
// Original createRandom__4itemPQ24item11descriptioniiii at 0x104440 and
// create__4itemPcPQ24item11descriptionii at 0x105f60. Not defined here.
int norrath_item_create_random(void* description, int level, int a, int b, int c);
unsigned norrath_item_create(char* name, void* description, int a, int b);
extern unsigned char norrath_ctors_lava_wraith_view_448fb0[];
extern unsigned char norrath_creature_view_484cac[];
extern unsigned char norrath_ctors_lava_wraith_view_4853d0[];
extern unsigned char norrath_ctors_lava_wraith_view_4853d8[];
unsigned norrath_construct_WeaponRack(void* object, const void* point, int type, char** arguments);
unsigned norrath_creature_construct(void* object, const void* point, short type, void* resource_map, float scale, char** arguments);
// Original setParameters__8Creaturefffffi at 0x1092f0, setAnimEventFlags__8Creatureififififi
// at 0x1128a8 and finishConstruction__8Creature at 0x1091b0. Not defined here.
void norrath_creature_set_parameters(void* object, float a, float b, float c, float d, float e, int flag);
void norrath_creature_set_anim_event_flags(void* object, int a, float b, int c, float d, int e, float f, int g, float h, int i);
unsigned norrath_creature_finish_construction(void* object);
void norrath_actor_animation_initialize(void* state);
void norrath_actor_animation_add(void* state, void* header, int flags);
unsigned long long norrath_actor_model_active_chunks(void* model, int requested);
extern unsigned char norrath_actor_view_4844a4[];
extern unsigned char norrath_ctors_lava_wraith_view_42d048[];
extern unsigned char norrath_ctors_lava_wraith_view_44f3d8[];
unsigned norrath_construct_Rondo(void* object, const void* point, short type, char** arguments);
void norrath_light_deactivate(void* light);
extern unsigned char norrath_ctors_lava_wraith_view_42c570[];
extern unsigned char norrath_ctors_lava_wraith_view_449550[];
extern unsigned char norrath_ctors_lava_wraith_view_4853b8[];
unsigned norrath_construct_Savepoint(void* object, const void* point, short type, char** arguments);
unsigned norrath_actor_add_slow_run_list(void* object);
void* norrath_list_oneway_link(void* head, void* object_head, void* value);
extern unsigned char norrath_ctors_lava_wraith_view_42bcb8[];
extern unsigned char norrath_ctors_lava_wraith_view_44aea0[];
extern unsigned char norrath_ctors_lava_wraith_view_42bd18[];
extern unsigned char norrath_ctors_lava_wraith_view_4852a8[];
unsigned norrath_construct_Wraith(void* object, const void* point, short type, char** arguments);
extern unsigned char norrath_ctors_lava_wraith_view_42b7e8[];
extern unsigned char norrath_ctors_lava_wraith_view_44e2b0[];
unsigned norrath_construct_SeaMonster(void* object, const void* point, short type, char** arguments);
extern char levelName[];
int norrath_factory_strcasecmp(const char* left, const char* right);
// Original setState__4Souli at 0x1f46c8. Not defined here.
void norrath_soul_set_state(void* object, int state);
extern unsigned char norrath_ctors_lava_wraith_view_42b3d8[];
extern unsigned char norrath_ctors_lava_wraith_view_44def0[];
extern unsigned char norrath_ctors_lava_wraith_view_485240[];
unsigned norrath_construct_Soul(void* object, const void* point, short type, char** arguments, void* minion);
// Original fptodp at 0x2fb320 (already named by the math package), dpadd at
// 0x2fb5c0 and dptofp at 0x2fbc20. The last two are named here by address
// because no verified authored counterpart is known. Not defined here.
unsigned long long norrath_entity_fptodp(float value);
unsigned long long norrath_softdouble_add_2fb5c0(unsigned long long left, unsigned long long right);
float norrath_softdouble_to_float_2fbc20(unsigned long long value);
extern unsigned char norrath_ctors_lava_wraith_view_42b330[];
extern unsigned char norrath_ctors_lava_wraith_view_44b630[];
unsigned norrath_construct_Scorpion(void* object, const void* point, short type, char** arguments);
extern unsigned char norrath_ctors_lava_wraith_view_42b478[];
extern unsigned char norrath_ctors_lava_wraith_view_44dfe0[];
unsigned norrath_construct_Ulthork(void* object, const void* point, short type, char** arguments);
int sprintf(char* destination, const char* format, ...);
char* strcpy(char* destination, const char* source);
unsigned norrath_object_add_to_delete_list(void* object);
// Original objectFindAnimation__FP10GameObjectPc at 0x152cc8 and
// animRandomizeTime__FP14AnimationState at 0x1601f8. Not defined here.
void* norrath_object_find_animation(void* object, char* name);
void norrath_actor_animation_randomize_time(void* state);
extern unsigned char norrath_ctors_lava_wraith_view_44adb0[];
extern unsigned char norrath_ctors_lava_wraith_view_4857e8[];
extern unsigned char norrath_ctors_lava_wraith_view_4857f0[];
extern unsigned char norrath_ctors_lava_wraith_view_472050[];
extern unsigned char norrath_ctors_lava_wraith_view_4857f8[];
extern unsigned char norrath_ctors_lava_wraith_view_485800[];
extern unsigned char norrath_ctors_lava_wraith_view_485808[];
extern unsigned char norrath_ctors_lava_wraith_view_485810[];
extern unsigned char norrath_ctors_lava_wraith_view_485818[];
extern unsigned char norrath_ctors_lava_wraith_view_485820[];
extern unsigned char norrath_ctors_lava_wraith_view_485828[];
extern unsigned char norrath_ctors_lava_wraith_view_472060[];
extern unsigned char norrath_ctors_lava_wraith_view_472040[];
unsigned norrath_construct_NPC(void* object, const void* point, short type, char** arguments);
// Original init__10StaticFire at 0x28df70. Not defined here.
void norrath_static_fire_init(void* object);
extern unsigned char norrath_ctors_lava_wraith_view_449310[];
extern unsigned char norrath_ctors_lava_wraith_view_486118[];
extern unsigned char norrath_ctors_lava_wraith_view_4778c8[];
extern unsigned char norrath_ctors_lava_wraith_view_4778d8[];
extern unsigned char norrath_ctors_lava_wraith_view_486120[];
extern unsigned char norrath_ctors_lava_wraith_view_486128[];
extern unsigned char norrath_ctors_lava_wraith_view_486130[];
unsigned norrath_construct_StaticFire(void* object, const void* point, short type, char** arguments);
float norrath_integer_sine(int angle);
float norrath_integer_cosine(int angle);
// Original worldHitscan__FP12_worldHeaderG6Point3T1fi at 0x17b7e0. Not defined here.
float norrath_world_hitscan(void* world, const void* from, const void* direction, float distance, int mode);
extern unsigned char norrath_ctors_lava_wraith_view_449c20[];
extern unsigned char norrath_ctors_lava_wraith_view_485330[];
extern unsigned char norrath_ctors_lava_wraith_view_470358[];
unsigned norrath_construct_WebTrap(void* object, const void* point, short type, char** arguments);
// Original dpmul at 0x2fb680, named by address like the other soft-double helpers. Not defined here.
unsigned long long norrath_softdouble_multiply_2fb680(unsigned long long left, unsigned long long right);
extern unsigned char norrath_ctors_lava_wraith_view_44a390[];
unsigned norrath_construct_WaterSpout(void* object, const void* point, int type);
extern unsigned char norrath_ctors_lava_wraith_view_42d650[];
extern unsigned char norrath_ctors_lava_wraith_view_44b080[];
unsigned norrath_construct_Vampire(void* object, const void* point, short type, char** arguments);
// Original initWeapon__8Skelight at 0x246598 and setState__8Skelighti at 0x246cf0. Not defined here.
void norrath_skelight_init_weapon(void* object);
void norrath_skelight_set_state(void* object, int state);
extern unsigned char norrath_ctors_lava_wraith_view_42dbf0[];
extern unsigned char norrath_ctors_lava_wraith_view_44fdf8[];
unsigned norrath_construct_Skelight(void* object, const void* point, short type, char** arguments, signed char flag, int value);
extern unsigned char norrath_ctors_lava_wraith_view_42ce60[];
extern unsigned char norrath_ctors_lava_wraith_view_44b260[];
unsigned norrath_construct_SuperOrc(void* object, const void* point, short type, char** arguments);
void* norrath_lump_load(char* lump_name);
extern unsigned char norrath_ctors_lava_wraith_view_42d700[];
extern unsigned char norrath_ctors_lava_wraith_view_44fd08[];
extern unsigned char norrath_ctors_lava_wraith_view_4858c0[];
extern unsigned char norrath_ctors_lava_wraith_view_4858c8[];
unsigned norrath_construct_VampireLord(void* object, const void* point, short type, char** arguments);
extern unsigned char norrath_ctors_lava_wraith_view_42d480[];
extern unsigned char norrath_ctors_lava_wraith_view_44f798[];
unsigned norrath_construct_MummyKing(void* object, const void* point, short type, char** arguments);
extern unsigned char norrath_ctors_lava_wraith_view_42d578[];
extern unsigned char norrath_ctors_lava_wraith_view_44fc18[];
unsigned norrath_construct_UndeadKnight(void* object, const void* point, short type, char** arguments);
extern unsigned char norrath_ctors_lava_wraith_view_44fb28[];
extern unsigned char norrath_ctors_lava_wraith_view_4858b8[];
unsigned norrath_construct_UndeadKnightClone(void* object, const void* point, short type, char** arguments);
extern unsigned char norrath_ctors_lava_wraith_view_42d138[];
extern unsigned char norrath_ctors_lava_wraith_view_44f5b8[];
unsigned norrath_construct_Minion(void* object, const void* point, short type, char** arguments);
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
