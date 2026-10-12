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
// Original __10GameObjectG6Point3PQ210GameObject11resourceMapPPc, lightAdd__FP5Light and
// objectAddToRunList__FP10GameObject. Defined by other packages or left unresolved.
unsigned norrath_game_object_construct(void* object, const void* point, void* resource_map, char** arguments);
void norrath_light_add(void* light);
unsigned norrath_actor_add_run_list(void* object);
unsigned norrath_construct_Lamp(void* object, const void* point);
// Original worldCheckForWater__FP12_worldHeaderG6Point3fPf. Not defined here.
int norrath_world_check_for_water(void* world, const void* point, float radius, float* output);
// Existing 4-byte `world` pointer at 0x4844a0.
extern unsigned char norrath_actors_b_view_4844a0[];
extern unsigned char norrath_ctors_ant_lantern_view_44e940[];
extern unsigned char norrath_ctors_ant_lantern_view_453328[];
extern unsigned char norrath_ctors_ant_lantern_view_44a810[];
extern unsigned char norrath_ctors_ant_lantern_view_481990[];
unsigned norrath_construct_Lantern(void* object, const void* point);
extern unsigned char norrath_ctors_ant_lantern_view_44a8a0[];
extern unsigned char norrath_ctors_ant_lantern_view_48198c[];
unsigned norrath_creature_construct(void* object, const void* point, short type, void* resource_map, float scale, char** arguments);
unsigned norrath_actor_set_parameters(void* object, float a, float b, float c, float d, float e, int flag);
char* norrath_actor_find_tag_string(void* object, char* tag, int occurrence);
int norrath_game_object_find_tag_int(void* object, char* tag, int occurrence);
unsigned norrath_actor_add_slow_run_list(void* object);
int norrath_factory_strcasecmp(const char* left, const char* right);
extern unsigned char norrath_actor_view_4844a4[];
unsigned norrath_construct_Generator(void* object, const void* point, short type, char** arguments);
extern unsigned char norrath_ctors_ant_lantern_view_44bbd0[];
extern unsigned char norrath_ctors_ant_lantern_view_46ded0[];
extern unsigned char norrath_ctors_ant_lantern_view_46dee0[];
extern unsigned char norrath_ctors_ant_lantern_view_46def0[];
extern unsigned char norrath_ctors_ant_lantern_view_485230[];
unsigned norrath_construct_Ice(void* object, const void* point, short type, char* first_lump, char* first_name, char* second_name, char* third_name, char* fourth_name);
void* norrath_lump_find_resource(char* lump_name, char* resource_name);
long long norrath_script_get_game_state(char* name);
extern unsigned char norrath_ctors_ant_lantern_view_44cc28[];
extern unsigned char norrath_ctors_ant_lantern_view_46d070[];
unsigned norrath_construct_FireFly(void* object, const void* point, short type, char** arguments);
unsigned norrath_construct_FireBeetle(void* object, const void* point, short type, char** arguments, int flag);
void norrath_actor_animation_initialize(void* state);
void norrath_actor_animation_randomize_time(void* state);
void norrath_actor_animation_add(void* state, void* header, int flags);
unsigned norrath_actor_finish_construction(void* object);
extern unsigned char norrath_ctors_ant_lantern_view_44bdb0[];
extern unsigned char norrath_ctors_ant_lantern_view_481540[];
extern unsigned char norrath_ctors_ant_lantern_view_481544[];
unsigned norrath_construct_Chest(void* object, const void* point, short type, char** arguments, int kind);
unsigned norrath_actor_random(void* object);
float norrath_player_find_tag_float(void* object, char* tag, int occurrence);
float norrath_player_treasure_ramp();
unsigned long long norrath_actor_model_active_chunks(void* model, int requested);
int strcmp(const char* left, const char* right);
extern unsigned char norrath_ctors_ant_lantern_view_42c8a0[];
extern unsigned char norrath_ctors_ant_lantern_view_44cb98[];
extern unsigned char norrath_ctors_ant_lantern_view_34b6b1[];
extern unsigned char norrath_ctors_ant_lantern_view_485398[];
extern unsigned char norrath_ctors_ant_lantern_view_485420[];
extern unsigned char norrath_ctors_ant_lantern_view_485438[];
extern unsigned char norrath_ctors_ant_lantern_view_485440[];
extern unsigned char norrath_ctors_ant_lantern_view_4844c0[];
unsigned norrath_construct_Gnome(void* object, const void* point, short type, char** arguments, char* name);
int strticmp__FPCcT0(const char* left, const char* right);
void initType__5Gnome(void* object);
extern unsigned char norrath_ctors_ant_lantern_view_42ba00[];
extern unsigned char norrath_ctors_ant_lantern_view_44b9f0[];
extern unsigned char norrath_ctors_ant_lantern_view_480b10[];
extern unsigned char norrath_ctors_ant_lantern_view_480b14[];
extern unsigned char norrath_ctors_ant_lantern_view_46f310[];
extern unsigned char norrath_ctors_ant_lantern_view_46f320[];
extern unsigned char norrath_ctors_ant_lantern_view_46f330[];
unsigned norrath_construct_Demon(void* object, const void* point, short type, char** arguments);
unsigned norrath_construct_Cyclops(void* object, const void* point, short type, char** arguments, int kind);
void setAnimEventFlags__8Creatureififififi(void* object, int a, float fa, int b, float fb, int c, float fc, int d, float fd, int e);
void initAttackVariant__5Demon(void* object);
extern unsigned char norrath_ctors_ant_lantern_view_42af70[];
extern unsigned char norrath_ctors_ant_lantern_view_44dd68[];
extern unsigned char norrath_ctors_ant_lantern_view_480ba8[];
extern unsigned char norrath_ctors_ant_lantern_view_480bac[];
extern unsigned char norrath_ctors_ant_lantern_view_480bb0[];
extern unsigned char norrath_ctors_ant_lantern_view_480bb4[];
extern unsigned char norrath_ctors_ant_lantern_view_42bb08[];
extern unsigned char norrath_ctors_ant_lantern_view_44bf90[];
extern unsigned char norrath_ctors_ant_lantern_view_481160[];
extern unsigned char norrath_ctors_ant_lantern_view_481164[];
extern unsigned char norrath_ctors_ant_lantern_view_481168[];
extern unsigned char norrath_ctors_ant_lantern_view_48116c[];
extern unsigned char norrath_ctors_ant_lantern_view_481170[];
extern unsigned char norrath_ctors_ant_lantern_view_481174[];
extern unsigned char norrath_ctors_ant_lantern_view_481178[];
extern unsigned char norrath_ctors_ant_lantern_view_48117c[];
extern unsigned char norrath_ctors_ant_lantern_view_42c308[];
extern unsigned char norrath_ctors_ant_lantern_view_44bea0[];
extern unsigned char norrath_ctors_ant_lantern_view_4814e4[];
extern unsigned char norrath_ctors_ant_lantern_view_4814e8[];
extern unsigned char norrath_ctors_ant_lantern_view_4814ec[];
extern unsigned char norrath_ctors_ant_lantern_view_4814f0[];
extern unsigned char norrath_ctors_ant_lantern_view_4814f4[];
extern unsigned char norrath_ctors_ant_lantern_view_4814f8[];
extern unsigned char norrath_ctors_ant_lantern_view_34b474[];
extern unsigned char norrath_creature_view_484cac[];
unsigned norrath_construct_AutoObject(void* object, char* name, const void* point, short type, char** arguments);
char* strcpy(char* destination, const char* source);
char* strcat(char* destination, const char* source);
extern unsigned char norrath_ctors_ant_lantern_view_44ece8[];
extern unsigned char norrath_ctors_ant_lantern_view_46fd38[];
extern unsigned char norrath_ctors_ant_lantern_view_4854c8[];
extern unsigned char norrath_ctors_ant_lantern_view_4854d0[];
extern unsigned char norrath_ctors_ant_lantern_view_4854d8[];
extern unsigned char norrath_ctors_ant_lantern_view_4854e0[];
extern unsigned char norrath_ctors_ant_lantern_view_4854e8[];
extern unsigned char norrath_ctors_ant_lantern_view_4854f0[];
extern unsigned char norrath_ctors_ant_lantern_view_4854f8[];
extern unsigned char norrath_ctors_ant_lantern_view_485500[];
unsigned norrath_construct_AutoProp(void* object, char* name, const void* point, short type, char** arguments);
int norrath_factory_sprintf(char* destination, const char* format, ...);
int norrath_world_find_closest_door(void* world, const void* point);
int norrath_world_set_door_collision(void* world, int door, int value);
int norrath_world_get_door_height(void* world, int door);
void norrath_animation_set_to_end(void* state);
void setPassableState__8AutoPropb(void* object, int passable);
void setState__8AutoPropi(void* object, int state);
extern unsigned char norrath_ctors_ant_lantern_view_44ec40[];
extern unsigned char norrath_ctors_ant_lantern_view_470df8[];
extern unsigned char norrath_ctors_ant_lantern_view_485530[];
extern unsigned char norrath_ctors_ant_lantern_view_470e08[];
extern unsigned char norrath_ctors_ant_lantern_view_470e18[];
extern unsigned char norrath_ctors_ant_lantern_view_470e28[];
extern unsigned char norrath_ctors_ant_lantern_view_470e38[];
extern unsigned char norrath_ctors_ant_lantern_view_470e48[];
extern unsigned char norrath_ctors_ant_lantern_view_485538[];
extern unsigned char norrath_ctors_ant_lantern_view_470e58[];
extern unsigned char norrath_ctors_ant_lantern_view_485540[];
extern unsigned char norrath_ctors_ant_lantern_view_470e68[];
extern unsigned char norrath_ctors_ant_lantern_view_470e78[];
extern unsigned char norrath_ctors_ant_lantern_view_470e88[];
extern unsigned char norrath_ctors_ant_lantern_view_470e98[];
extern unsigned char norrath_ctors_ant_lantern_view_485480[];
extern unsigned char norrath_ctors_ant_lantern_view_485488[];
extern unsigned char norrath_ctors_ant_lantern_view_470ea8[];
extern unsigned char norrath_ctors_ant_lantern_view_470eb8[];
extern unsigned char norrath_ctors_ant_lantern_view_485548[];
extern unsigned char norrath_ctors_ant_lantern_view_481704[];
extern unsigned char norrath_ctors_ant_lantern_view_481708[];
unsigned norrath_construct_AutoPropLP(void* object, char* name, const void* point, short type, char** arguments);
int hasLightOrParticleNumber__FPPci(char** arguments, int number);
unsigned __11AddParticleG6Point3PPci(void* object, const void* point, char** arguments, int number);
extern unsigned char norrath_ctors_ant_lantern_view_44e9d0[];
extern unsigned char norrath_ctors_ant_lantern_view_4711f0[];
extern unsigned char norrath_ctors_ant_lantern_view_471208[];
extern unsigned char norrath_ctors_ant_lantern_view_471218[];
extern unsigned char norrath_ctors_ant_lantern_view_471230[];
extern unsigned char norrath_ctors_ant_lantern_view_485760[];
extern unsigned char norrath_ctors_ant_lantern_view_471248[];
extern unsigned char norrath_ctors_ant_lantern_view_471260[];
extern unsigned char norrath_ctors_ant_lantern_view_485768[];
extern unsigned char norrath_ctors_ant_lantern_view_471278[];
unsigned norrath_construct_Blocker(void* object, const void* point, short type, char** arguments);
float icos__Fi(int angle);
float isin__Fi(int angle);
extern unsigned char norrath_ctors_ant_lantern_view_44ebb0[];
extern unsigned char norrath_ctors_ant_lantern_view_470ff0[];
extern unsigned char norrath_ctors_ant_lantern_view_471000[];
extern unsigned char norrath_ctors_ant_lantern_view_471010[];
extern unsigned char norrath_ctors_ant_lantern_view_471020[];
extern unsigned char norrath_ctors_ant_lantern_view_485560[];
extern unsigned char norrath_ctors_ant_lantern_view_485568[];
extern unsigned char norrath_ctors_ant_lantern_view_485570[];
extern unsigned char norrath_ctors_ant_lantern_view_485578[];
extern unsigned char norrath_ctors_ant_lantern_view_471030[];
extern unsigned char norrath_ctors_ant_lantern_view_485580[];
extern unsigned char norrath_ctors_ant_lantern_view_471150[];
unsigned norrath_construct_AntQueen(void* object, const void* point, short type, char** arguments);
unsigned norrath_construct_Cthulu(void* object, const void* point, short type, char** arguments);
unsigned long long norrath_entity_fptodp(float value);
unsigned long long dpadd(unsigned long long left, unsigned long long right);
float dptofp(unsigned long long value);
extern unsigned char norrath_ctors_ant_lantern_view_42bf58[];
extern unsigned char norrath_ctors_ant_lantern_view_44c440[];
extern unsigned char norrath_ctors_ant_lantern_view_4812d8[];
extern unsigned char norrath_ctors_ant_lantern_view_4812dc[];
extern unsigned char norrath_ctors_ant_lantern_view_4812e0[];
extern unsigned char norrath_ctors_ant_lantern_view_4812e4[];
extern unsigned char norrath_ctors_ant_lantern_view_4812e8[];
extern unsigned char norrath_ctors_ant_lantern_view_4812ec[];
extern unsigned char norrath_ctors_ant_lantern_view_438a38[];
extern unsigned char norrath_ctors_ant_lantern_view_450160[];
extern unsigned char norrath_ctors_ant_lantern_view_482328[];
extern unsigned char norrath_ctors_ant_lantern_view_48232c[];
extern unsigned char norrath_ctors_ant_lantern_view_482330[];
extern unsigned char norrath_ctors_ant_lantern_view_482334[];
extern unsigned char norrath_ctors_ant_lantern_view_482338[];
extern unsigned char norrath_ctors_ant_lantern_view_48233c[];
extern unsigned char norrath_ctors_ant_lantern_view_482340[];
extern unsigned char norrath_ctors_ant_lantern_view_476ac8[];
unsigned norrath_construct_Arenabeast(void* object, const void* point, short type, char** arguments);
unsigned norrath_construct_Ghoul(void* object, const void* point, short type, int variant, char** arguments);
extern unsigned char norrath_ctors_ant_lantern_view_42d200[];
extern unsigned char norrath_ctors_ant_lantern_view_44f6a8[];
extern unsigned char norrath_ctors_ant_lantern_view_481ab4[];
extern unsigned char norrath_ctors_ant_lantern_view_481ab8[];
extern unsigned char norrath_ctors_ant_lantern_view_481abc[];
extern unsigned char norrath_ctors_ant_lantern_view_481ac0[];
extern unsigned char norrath_ctors_ant_lantern_view_481ac4[];
extern unsigned char norrath_ctors_ant_lantern_view_481ac8[];
extern unsigned char norrath_ctors_ant_lantern_view_42b630[];
extern unsigned char norrath_ctors_ant_lantern_view_42b548[];
extern unsigned char norrath_ctors_ant_lantern_view_44bae0[];
extern unsigned char norrath_ctors_ant_lantern_view_480e58[];
extern unsigned char norrath_ctors_ant_lantern_view_480e5c[];
extern unsigned char norrath_ctors_ant_lantern_view_480e60[];
extern unsigned char norrath_ctors_ant_lantern_view_480e64[];
extern unsigned char norrath_ctors_ant_lantern_view_480e68[];
extern unsigned char norrath_ctors_ant_lantern_view_480e6c[];
extern unsigned char norrath_ctors_ant_lantern_view_480e70[];
unsigned norrath_construct_Badger(void* object, const void* point, short type, char** arguments, int kind);
unsigned norrath_construct_BlackWidow(void* object, const void* point, short type, char** arguments, char* name);
void __3Webii(void* object, int first, int second);
extern unsigned char norrath_ctors_ant_lantern_view_42b1d0[];
extern unsigned char norrath_ctors_ant_lantern_view_44c350[];
extern unsigned char norrath_ctors_ant_lantern_view_46dbe0[];
extern unsigned char norrath_ctors_ant_lantern_view_480cc8[];
extern unsigned char norrath_ctors_ant_lantern_view_480ccc[];
extern unsigned char norrath_ctors_ant_lantern_view_480cd0[];
extern unsigned char norrath_ctors_ant_lantern_view_480cd4[];
extern unsigned char norrath_ctors_ant_lantern_view_480cd8[];
extern unsigned char norrath_ctors_ant_lantern_view_480cdc[];
extern unsigned char norrath_ctors_ant_lantern_view_480ce0[];
extern unsigned char norrath_ctors_ant_lantern_view_480ce4[];
extern unsigned char norrath_ctors_ant_lantern_view_480ce8[];
extern unsigned char norrath_ctors_ant_lantern_view_480cec[];
extern unsigned char norrath_ctors_ant_lantern_view_42c188[];
extern unsigned char norrath_ctors_ant_lantern_view_44c260[];
extern unsigned char norrath_ctors_ant_lantern_view_46ffe0[];
extern unsigned char norrath_ctors_ant_lantern_view_4813bc[];
extern unsigned char norrath_ctors_ant_lantern_view_4813c0[];
extern unsigned char norrath_ctors_ant_lantern_view_4813c4[];
extern unsigned char norrath_ctors_ant_lantern_view_4813c8[];
extern unsigned char norrath_ctors_ant_lantern_view_4813cc[];
extern unsigned char norrath_ctors_ant_lantern_view_4813d0[];
extern unsigned char norrath_ctors_ant_lantern_view_4813d4[];
unsigned norrath_construct_Froglock(void* object, const void* point, short type, char** arguments, char* name);
unsigned norrath_construct_Catapult(void* object, const void* point, short type, char** arguments);
void initAttackVariant__8Froglock(void* object);
void* norrath_list_oneway_link(void* head, void* object_head, void* value);
extern unsigned char norrath_ctors_ant_lantern_view_42b958[];
extern unsigned char norrath_ctors_ant_lantern_view_44bcc0[];
extern unsigned char norrath_ctors_ant_lantern_view_46f120[];
extern unsigned char norrath_ctors_ant_lantern_view_46f130[];
extern unsigned char norrath_ctors_ant_lantern_view_48109c[];
extern unsigned char norrath_ctors_ant_lantern_view_4810a0[];
extern unsigned char norrath_ctors_ant_lantern_view_4810a4[];
extern unsigned char norrath_ctors_ant_lantern_view_4810a8[];
extern unsigned char norrath_ctors_ant_lantern_view_4810ac[];
extern unsigned char norrath_ctors_ant_lantern_view_4810b0[];
extern unsigned char norrath_ctors_ant_lantern_view_438eb8[];
extern unsigned char norrath_ctors_ant_lantern_view_4506b0[];
extern unsigned char norrath_ctors_ant_lantern_view_4777c8[];
extern unsigned char norrath_ctors_ant_lantern_view_4860e0[];
extern unsigned char norrath_ctors_ant_lantern_view_4860e8[];
extern unsigned char norrath_ctors_ant_lantern_view_4860f0[];
extern unsigned char norrath_ctors_ant_lantern_view_4860f8[];
extern unsigned char norrath_ctors_ant_lantern_view_477798[];
extern unsigned char norrath_ctors_ant_lantern_view_4777d8[];
extern unsigned char norrath_ctors_ant_lantern_view_4825bc[];
extern unsigned char norrath_ctors_ant_lantern_view_4825c0[];
unsigned norrath_construct_Innoruuk(void* object, const void* point, short type, char** arguments);
unsigned norrath_construct_JumpGate(void* object, const void* point, short type, char** arguments);
void addIcon__3mapP6Point3iPciT3N33(void* point, int first, char* text, int second, char* third, char* fourth, char* fifth, char* sixth);
void lightActivate__FP5Light(void* light);
void lightDeactivate__FP5Light(void* light);
void objectFindEmptySpot__FG6Point3iiPc(void* result, const void* point, int first, int second, char* third);
extern unsigned char norrath_ctors_ant_lantern_view_42b870[];
extern unsigned char norrath_ctors_ant_lantern_view_44e550[];
extern unsigned char norrath_ctors_ant_lantern_view_480f4c[];
extern unsigned char norrath_ctors_ant_lantern_view_480f50[];
extern unsigned char norrath_ctors_ant_lantern_view_480f54[];
extern unsigned char norrath_ctors_ant_lantern_view_480f58[];
extern unsigned char norrath_ctors_ant_lantern_view_46edb0[];
extern unsigned char norrath_ctors_ant_lantern_view_480f5c[];
extern unsigned char norrath_ctors_ant_lantern_view_480f60[];
extern unsigned char norrath_ctors_ant_lantern_view_480f64[];
extern unsigned char norrath_ctors_ant_lantern_view_485270[];
extern unsigned char norrath_ctors_ant_lantern_view_480f68[];
extern unsigned char norrath_ctors_ant_lantern_view_480f6c[];
extern unsigned char norrath_ctors_ant_lantern_view_480f70[];
extern unsigned char norrath_ctors_ant_lantern_view_480f74[];
extern unsigned char norrath_ctors_ant_lantern_view_480f78[];
extern unsigned char norrath_ctors_ant_lantern_view_480f7c[];
extern unsigned char norrath_ctors_ant_lantern_view_480f80[];
extern unsigned char norrath_ctors_ant_lantern_view_480f84[];
extern unsigned char norrath_ctors_ant_lantern_view_4494c0[];
extern unsigned char norrath_ctors_ant_lantern_view_476b70[];
extern unsigned char norrath_ctors_ant_lantern_view_486038[];
extern unsigned char norrath_ctors_ant_lantern_view_486040[];
extern unsigned char norrath_ctors_ant_lantern_view_476b80[];
extern unsigned char norrath_ctors_ant_lantern_view_476b90[];
extern unsigned char norrath_ctors_ant_lantern_view_486048[];
extern unsigned char norrath_ctors_ant_lantern_view_486050[];
extern unsigned char norrath_ctors_ant_lantern_view_486058[];
extern unsigned char norrath_ctors_ant_lantern_view_486060[];
extern unsigned char norrath_ctors_ant_lantern_view_48239c[];
extern unsigned char norrath_ctors_ant_lantern_view_486030[];
extern unsigned char norrath_ctors_ant_lantern_view_486da4[];
extern unsigned char norrath_ctors_ant_lantern_view_484538[];
extern unsigned char norrath_ctors_ant_lantern_view_483e5c[];
extern unsigned char norrath_ctors_ant_lantern_view_484f38[];
extern unsigned char norrath_ctors_ant_lantern_view_484ca8[];
extern unsigned char norrath_ctors_ant_lantern_view_429410[];
// Vtable objects. Slots stay unresolved.
extern unsigned char norrath_ctors_ant_lantern_view_44ea78[];
}
namespace norrath_ctors_ant_lantern {
using Address = __UINTPTR_TYPE__;
inline Address address(const void* p) { return reinterpret_cast<Address>(p); }
inline unsigned address_bits(const void* p) { return static_cast<unsigned>(address(p)); }
inline unsigned char* bytes(Address a) { return reinterpret_cast<unsigned char*>(a); }
#if defined(NORRATH_CTORS_ANT_LANTERN_HOST)
// Host pointers do not fit the 32-bit fields this code reloads. Tests record
// the full pointer; EE builds keep the direct cast below.
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
    for (unsigned index = 0u; index < 64u; ++index)
        if (host_rows[index].full != nullptr && host_rows[index].bits == bits) return host_rows[index].full;
    return reinterpret_cast<void*>(a);
}
#else
inline void* pointer(Address a) { return reinterpret_cast<void*>(a); }
#endif
inline void write_half(Address a, unsigned value) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
}
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
inline float read_single(const unsigned char* view) {
    const unsigned bits = read_word(address(view));
    float value;
    __builtin_memcpy(&value, &bits, 4);
    return value;
}
struct PointBytes { unsigned x, y, z; };
inline PointBytes snapshot_point(const void* point) {
    const Address a = address(point);
    return PointBytes{read_word(a), read_word(a + 4u), read_word(a + 8u)};
}
}
static_assert(sizeof(norrath_ctors_ant_lantern::PointBytes) == 12 &&
              alignof(norrath_ctors_ant_lantern::PointBytes) == 4, "raw 12-byte point storage");
