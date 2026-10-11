// Observed __7VehicleG6Point3sPPci at 0x2702d8, extent 700.
// GameObject receives a 12-byte point snapshot, a null resource map, and the
// argument list. The short is kept and later stored at halves 56 and 1048.
// Byte 125 becomes 8, _vt$7Vehicle (0x44ffd8) is stored at object+72, and word
// 2136 is cleared. findTagString uses the text "lump" and occurrence 0. The
// copy is formatted as "%s.lmp" at object+76, "%s.vif", and "%s.tex".
// Object+12 is ORed with 0x00100001. A zero fifth argument asks
// worldFindHeight for the point at object+16 with radius 32 and stores that
// result plus 19.5 at object+24. A nonzero argument skips that store.
// The two lump lookups use the formatted names. modelGetActiveChunks is called
// with the first lookup and integer 0; its 8-byte result is stored at
// object+392 before objectAddToRunList. Word 976 becomes 4, then
// addPassenger(object, -1). Three memset ranges follow the explicit clears:
// 16 bytes at 984, 32 at 2104, 32 at 1052, and 16 at 2088. Twelve floats at
// object+1932 are cleared. vehicleCount is stored at object+2156, the object
// is written into vehicleList at that index, and the count increments before
// processParams. processParams receives the word reloaded from object+44.
// The 12 bytes at object+16 are copied to object+1632. gameLineInitPathState
// receives object+2272, the text "path", and object+16. Low V0 is that call.
extern "C" unsigned norrath_game_object_construct(void* object, const void* point, void* resource_map, char** arguments);
extern "C" char* norrath_actor_find_tag_string(void* object, char* tag, int occurrence);
extern "C" char* strcpy(char* destination, const char* source);
extern "C" int sprintf(char* destination, const char* format, ...);
extern "C" float norrath_world_find_height(unsigned char* world, const unsigned char* point, float radius);
extern "C" void* lumpFindResource__FPcT0(char* group, char* name);
extern "C" unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int which);
extern "C" unsigned norrath_actor_add_run_list(void* object);
extern "C" unsigned addPassenger__7Vehiclei(void* object, int seat);
extern "C" void* memset(void* destination, int value, unsigned size);
extern "C" unsigned processParams__7VehiclePPc(void* object, char** arguments);
extern "C" unsigned gameLineInitPathState__FP17gameLinePathStatePcP6Point3(void* state, char* name, void* point);
extern "C" unsigned char norrath_actors_b_view_4844a0[];
extern "C" unsigned char norrath_vehicle_view_44ffd8[];

namespace {
using Address = __UINTPTR_TYPE__;
__attribute__((always_inline)) inline Address address(const void* p) { return reinterpret_cast<Address>(p); }
__attribute__((always_inline)) inline unsigned address_bits(const void* p) { return static_cast<unsigned>(address(p)); }
__attribute__((always_inline)) inline unsigned char* bytes(Address a) { return reinterpret_cast<unsigned char*>(a); }
__attribute__((always_inline)) inline void* pointer(Address a) { return reinterpret_cast<void*>(a); }
__attribute__((always_inline)) inline unsigned read_word(Address a) {
    const unsigned char* p = bytes(a);
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
__attribute__((always_inline)) inline void write_word(Address a, unsigned value) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
__attribute__((always_inline)) inline void write_half(Address a, unsigned value) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
}
__attribute__((always_inline)) inline float bits_to_float(unsigned bits) {
    float value;
    unsigned char* p = reinterpret_cast<unsigned char*>(&value);
    p[0] = static_cast<unsigned char>(bits);
    p[1] = static_cast<unsigned char>(bits >> 8);
    p[2] = static_cast<unsigned char>(bits >> 16);
    p[3] = static_cast<unsigned char>(bits >> 24);
    return value;
}
__attribute__((always_inline)) inline void write_float(Address a, float value) {
    const unsigned char* p = reinterpret_cast<const unsigned char*>(&value);
    write_word(a, unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24));
}
struct PointBytes { unsigned x, y, z; };
__attribute__((always_inline)) inline PointBytes snapshot_point(const void* point) {
    const Address a = address(point);
    return PointBytes{read_word(a), read_word(a + 4u), read_word(a + 8u)};
}
}
static_assert(sizeof(PointBytes) == 12, "12-byte point");

extern "C" {
const char norrath_vehicle_lump_tag[] = "lump";
const char norrath_vehicle_lump_format[] = "%s.lmp";
const char norrath_vehicle_vif_format[] = "%s.vif";
const char norrath_vehicle_tex_format[] = "%s.tex";
const char norrath_vehicle_path_tag[] = "path";
unsigned norrath_vehicle_count_word;
unsigned char norrath_vehicle_list_bytes[32 * 4];
}

extern "C" unsigned norrath_construct_Vehicle(void* object, const void* point, short type, char** arguments, int flag) {
    const PointBytes local = snapshot_point(point);
    const unsigned saved_type = static_cast<unsigned>(type);
    (void)norrath_game_object_construct(object, &local, nullptr, arguments);
    const Address a = address(object);
    bytes(a)[125] = 8u;
    write_word(a + 72u, address_bits(norrath_vehicle_view_44ffd8));
    write_word(a + 2136u, 0u);
    char copied[32];
    char vif_name[128];
    char tex_name[144];
    char* tag = norrath_actor_find_tag_string(object, const_cast<char*>(norrath_vehicle_lump_tag), 0);
    strcpy(copied, tag);
    sprintf(reinterpret_cast<char*>(pointer(a + 76u)), norrath_vehicle_lump_format, copied);
    sprintf(vif_name, norrath_vehicle_vif_format, copied);
    sprintf(tex_name, norrath_vehicle_tex_format, copied);
    write_word(a + 12u, read_word(a + 12u) | 0x00100001u);
    if (flag == 0) {
        const PointBytes position = snapshot_point(pointer(a + 16u));
        const float found = norrath_world_find_height(reinterpret_cast<unsigned char*>(pointer(read_word(address(norrath_actors_b_view_4844a0)))),
                                                      reinterpret_cast<const unsigned char*>(&position), bits_to_float(0x42000000u));
        write_float(a + 24u, found + bits_to_float(0x419c0000u));
    }
    void* vif = lumpFindResource__FPcT0(reinterpret_cast<char*>(pointer(a + 76u)), vif_name);
    write_word(a + 112u, address_bits(vif));
    void* tex = lumpFindResource__FPcT0(reinterpret_cast<char*>(pointer(a + 76u)), tex_name);
    write_word(a + 116u, address_bits(tex));
    const unsigned long long chunks = modelGetActiveChunks__FP12_modelHeaderi(pointer(read_word(a + 112u)), 0);
    write_word(a + 392u, static_cast<unsigned>(chunks));
    write_word(a + 396u, static_cast<unsigned>(chunks >> 32));
    (void)norrath_actor_add_run_list(object);
    write_word(a + 976u, 4u);
    (void)addPassenger__7Vehiclei(object, -1);
    (void)memset(pointer(a + 984u), 0, 16u);
    write_word(a + 1024u, 0u);
    write_word(a + 2148u, 0u);
    write_word(a + 956u, 0xffffffffu);
    write_word(a + 820u, 0u);
    write_word(a + 1000u, 0u);
    write_word(a + 2168u, 0u);
    (void)memset(pointer(a + 2104u), 0, 32u);
    (void)memset(pointer(a + 1052u), 0, 32u);
    (void)memset(pointer(a + 2088u), 0, 16u);
    for (unsigned step = 0u; step < 4u; ++step) {
        const unsigned base = 1932u + step * 12u;
        write_float(a + base, 0.0f);
        write_float(a + base + 4u, 0.0f);
        write_float(a + base + 8u, 0.0f);
    }
    const unsigned count = norrath_vehicle_count_word;
    write_word(a + 2156u, count);
    write_word(address(norrath_vehicle_list_bytes) + (count << 2), address_bits(object));
    norrath_vehicle_count_word = count + 1u;
    (void)processParams__7VehiclePPc(object, reinterpret_cast<char**>(pointer(read_word(a + 44u))));
    write_word(a + 1632u, read_word(a + 16u));
    write_word(a + 1636u, read_word(a + 20u));
    write_word(a + 1640u, read_word(a + 24u));
    write_half(a + 56u, saved_type);
    write_half(a + 1048u, saved_type);
    return gameLineInitPathState__FP17gameLinePathStatePcP6Point3(pointer(a + 2272u), const_cast<char*>(norrath_vehicle_path_tag), pointer(a + 16u));
}
