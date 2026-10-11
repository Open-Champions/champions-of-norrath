// Host contract for norrath_construct_Vehicle. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" unsigned norrath_construct_Vehicle(void* object, const void* point, short type, char** arguments, int flag);
extern "C" {
unsigned char norrath_vehicle_view_44ffd8[4];
unsigned char norrath_actors_b_view_4844a0[4];
}
extern "C" unsigned norrath_vehicle_count_word;
extern "C" unsigned char norrath_vehicle_list_bytes[128];

static int failures = 0;
static const char* calls[16];
static int step = 0;
static const void* seen_point = nullptr;
static unsigned seen_point_bytes[3];
static void* seen_map = reinterpret_cast<void*>(1);
static char** seen_arguments = nullptr;
static char* seen_tag = nullptr;
static int seen_occurrence = -1;
static float seen_radius = -1.0f;
static unsigned seen_height_point[3];
static void* seen_model = nullptr;
static int seen_which = -1;
static int seen_seat = 0;
static unsigned passenger_flags = 0;
static unsigned memset_offset[4];
static size_t memset_size[4];
static int memset_count = 0;
static unsigned params_count = 0xffffffffu;
static void* params_arguments = nullptr;
static char seen_path[8];
static unsigned path_point[3];
static int height_calls = 0;
static char first_group[16];
static char first_name[16];
static char second_name[16];
static unsigned char object[2400];

static void fill(unsigned char* p, unsigned n, unsigned char value) {
    for (unsigned i = 0; i < n; ++i) p[i] = value;
}
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static void note(const char* name) {
    if (step < 16) calls[step] = name;
    ++step;
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
static unsigned float_bits(float value) {
    const unsigned char* p = reinterpret_cast<const unsigned char*>(&value);
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }

extern "C" unsigned norrath_game_object_construct(void* storage, const void* point, void* resource_map, char** arguments) {
    note("object");
    seen_point = point;
    std::memcpy(seen_point_bytes, point, 12);
    seen_map = resource_map;
    seen_arguments = arguments;
    write_word(static_cast<unsigned char*>(storage) + 12, 0x8u);
    write_word(static_cast<unsigned char*>(storage) + 16, 0x10u);
    write_word(static_cast<unsigned char*>(storage) + 20, 0x20u);
    write_word(static_cast<unsigned char*>(storage) + 24, 0x30u);
    write_word(static_cast<unsigned char*>(storage) + 44, 0x44u);
    return 1u;
}
extern "C" char* norrath_actor_find_tag_string(void* storage, char* tag, int occurrence) {
    (void)storage;
    note("tag");
    seen_tag = tag;
    seen_occurrence = occurrence;
    return const_cast<char*>("boat");
}
extern "C" float norrath_world_find_height(unsigned char* world, const unsigned char* point, float radius) {
    note("height");
    ++height_calls;
    expect(world == reinterpret_cast<unsigned char*>(0x30ul), "height world");
    seen_radius = radius;
    std::memcpy(seen_height_point, point, 12);
    return 10.0f;
}
extern "C" void* lumpFindResource__FPcT0(char* group, char* name) {
    note("lump");
    if (std::strcmp(name, "boat.vif") == 0) {
        std::snprintf(first_group, sizeof first_group, "%s", group);
        std::snprintf(first_name, sizeof first_name, "%s", name);
        return reinterpret_cast<void*>(0x1111ul);
    }
    std::snprintf(second_name, sizeof second_name, "%s", name);
    return reinterpret_cast<void*>(0x2222ul);
}
extern "C" unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int which) {
    note("chunks");
    seen_model = model;
    seen_which = which;
    return 0x99aabbccddeeff00ull;
}
extern "C" unsigned norrath_actor_add_run_list(void* storage) {
    note("run");
    expect(storage == object, "run list object");
    expect(read_word(object + 392) == 0xddeeff00u && read_word(object + 396) == 0x99aabbccu, "chunks stored before run list");
    return 0x70u;
}
extern "C" unsigned addPassenger__7Vehiclei(void* storage, int seat) {
    note("passenger");
    expect(storage == object, "passenger object");
    seen_seat = seat;
    passenger_flags = read_word(object + 976);
    return 0x71u;
}
extern "C" void* memset(void* destination, int value, size_t size) {
    note("memset");
    const auto* start = static_cast<unsigned char*>(destination);
    expect(start >= object && start < object + sizeof object, "memset stays in the object");
    memset_offset[memset_count] = static_cast<unsigned>(start - object);
    memset_size[memset_count] = size;
    expect(value == 0, "memset value");
    ++memset_count;
    fill(static_cast<unsigned char*>(destination), size, 0);
    return destination;
}
extern "C" unsigned processParams__7VehiclePPc(void* storage, char** arguments) {
    note("params");
    expect(storage == object, "params object");
    params_count = norrath_vehicle_count_word;
    params_arguments = arguments;
    return 0x72u;
}
extern "C" unsigned gameLineInitPathState__FP17gameLinePathStatePcP6Point3(void* state, char* name, void* point) {
    note("path");
    expect(state == object + 2272, "path state");
    std::snprintf(seen_path, sizeof seen_path, "%s", name);
    std::memcpy(path_point, point, 12);
    return 0x909u;
}

static void reset_log() {
    step = 0;
    height_calls = 0;
    memset_count = 0;
    for (int i = 0; i < 16; ++i) calls[i] = "";
}

int main() {
    unsigned point[3] = {1u, 2u, 3u};
    char* arguments[] = {const_cast<char*>("arg"), nullptr};
    write_word(norrath_actors_b_view_4844a0, 0x30u);
    norrath_vehicle_count_word = 4u;
    fill(object, sizeof object, 0x5a);
    fill(object + 1931, 50, 0xab);
    reset_log();
    const unsigned result = norrath_construct_Vehicle(object, point, static_cast<short>(-2), arguments, 0);
    expect(result == 0x909u, "path result");
    expect(seen_point != point && seen_point_bytes[0] == 1u && seen_point_bytes[1] == 2u && seen_point_bytes[2] == 3u, "point snapshot");
    expect(seen_map == nullptr && seen_arguments == arguments, "null map and arguments");
    expect(object[125] == 8u, "byte 125");
    expect(read_word(object + 72) == static_cast<unsigned>(reinterpret_cast<unsigned long>(norrath_vehicle_view_44ffd8)), "vtable");
    expect(read_word(object + 2136) == 0u, "word 2136");
    expect(seen_occurrence == 0 && std::strcmp(seen_tag, "lump") == 0, "lump tag");
    expect(std::strcmp(reinterpret_cast<char*>(object + 76), "boat.lmp") == 0, "lump name");
    expect(std::strcmp(first_group, "boat.lmp") == 0 && std::strcmp(first_name, "boat.vif") == 0, "vif lookup");
    expect(std::strcmp(second_name, "boat.tex") == 0, "tex lookup");
    expect(read_word(object + 12) == (0x8u | 0x00100001u), "flags");
    expect(height_calls == 1 && seen_radius == 32.0f, "height radius");
    expect(seen_height_point[0] == 0x10u && seen_height_point[1] == 0x20u && seen_height_point[2] == 0x30u, "height point");
    expect(read_word(object + 24) == float_bits(10.0f + 19.5f), "raised height");
    expect(read_word(object + 112) == 0x1111u && read_word(object + 116) == 0x2222u, "resource words");
    expect(seen_model == reinterpret_cast<void*>(0x1111ul) && seen_which == 0, "chunk query");
    expect(seen_seat == -1 && passenger_flags == 4u, "passenger");
    expect(memset_count == 4, "four clears");
    expect(memset_offset[0] == 984u && memset_size[0] == 16u, "first clear");
    expect(memset_offset[1] == 2104u && memset_size[1] == 32u, "second clear");
    expect(memset_offset[2] == 1052u && memset_size[2] == 32u, "third clear");
    expect(memset_offset[3] == 2088u && memset_size[3] == 16u, "fourth clear");
    expect(read_word(object + 1024) == 0u && read_word(object + 2148) == 0u && read_word(object + 956) == 0xffffffffu, "explicit words");
    expect(read_word(object + 820) == 0u && read_word(object + 1000) == 0u && read_word(object + 2168) == 0u, "more explicit words");
    expect(object[1931] == 0xabu && object[1980] == 0xabu, "float clear stays inside 1932..1979");
    for (unsigned offset = 1932u; offset < 1980u; offset += 4u) expect(read_word(object + offset) == 0u, "cleared float");
    expect(read_word(object + 2156) == 4u && norrath_vehicle_count_word == 5u, "count advances");
    expect(params_count == 5u && params_arguments == reinterpret_cast<char**>(0x44ul), "params sees the new count");
    expect(read_word(norrath_vehicle_list_bytes + 16) == static_cast<unsigned>(reinterpret_cast<unsigned long>(object)), "list slot");
    expect(read_word(object + 1632) == 0x10u && read_word(object + 1636) == 0x20u, "copied position");
    expect(read_word(object + 1640) == float_bits(10.0f + 19.5f), "copied raised height");
    expect(half(object + 56) == 0xfffeu && half(object + 1048) == 0xfffeu, "saved type");
    expect(std::strcmp(seen_path, "path") == 0 && path_point[0] == 0x10u, "path point");
    const char* expect_calls[] = {"object", "tag", "height", "lump", "lump", "chunks", "run", "passenger",
                                  "memset", "memset", "memset", "memset", "params", "path"};
    expect(step == 14, "call count");
    for (int i = 0; i < 14; ++i) expect(std::strcmp(calls[i], expect_calls[i]) == 0, expect_calls[i]);

    point[0] = 9u;
    fill(object, sizeof object, 0x5a);
    reset_log();
    const unsigned skipped = norrath_construct_Vehicle(object, point, static_cast<short>(4), arguments, -1);
    expect(skipped == 0x909u && height_calls == 0, "nonzero flag skips height");
    expect(read_word(object + 24) == 0x30u, "height word stays");
    expect(half(object + 56) == 4u && norrath_vehicle_count_word == 6u, "second insert");
    expect(read_word(object + 1632) == 0x10u && read_word(object + 1640) == 0x30u, "unraised copy");

    if (failures != 0) return 1;
    std::printf("vehicle constructor passed\n");
    return 0;
}
