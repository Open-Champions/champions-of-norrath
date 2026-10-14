#include "../switch.cpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int stage;
static bool tag_present;
static char** seen_arguments;
static const void* seen_point;
static void* seen_map;
static unsigned char seen_bytes[12];
static unsigned flags_at_door;
static void* door_world;
static void* height_world;
static unsigned char door_point[12];
static int collision_index;
static int height_index;
static int delete_calls;
static int scripted_door;
static int scripted_height;
static unsigned char* active_object;
static unsigned flags_during_door;
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
extern "C" unsigned norrath_game_object_construct(void* object, const void* point, void* resource_map, char** arguments) {
    expect(stage == 0, "game object first");
    stage = 1;
    seen_arguments = arguments;
    seen_map = resource_map;
    seen_point = point;
    std::memcpy(seen_bytes, point, 12);
    auto* raw = reinterpret_cast<unsigned char*>(object);
    std::memcpy(raw + 16, point, 12);
    raw[16] = 0x11;
    active_object = raw;
    return 0x111u;
}
extern "C" char* norrath_actor_find_tag_string(void* object, char* tag, int occurrence) {
    expect(stage == 1 && occurrence == 0, "tag before doors");
    stage = 2;
    (void)object;
    expect(tag == reinterpret_cast<char*>(norrath_actors_a_view_485358), "switch tag");
    expect(word_at(active_object + 72) == static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_actors_a_view_44d0a8)),
           "first view before the door queries");
    return tag_present ? tag : nullptr;
}
extern "C" int norrath_world_find_closest_door(void* world, const void* point) {
    expect(stage == 2, "closest door after tag");
    stage = 3;
    door_world = world;
    std::memcpy(door_point, point, 12);
    flags_during_door = word_at(active_object + 12);
    return scripted_door;
}
extern "C" int norrath_world_set_door_collision(void* world, int index, int value) {
    expect(stage == 3 && world == door_world && value == 1, "collision after closest door");
    stage = 4;
    collision_index = index;
    return 0x222u;
}
extern "C" int norrath_world_get_door_height(void* world, int index) {
    expect(stage == 4 && world == door_world, "height after collision");
    stage = 5;
    height_world = world;
    height_index = index;
    return scripted_height;
}
extern "C" unsigned norrath_object_add_to_delete_list(void*) {
    expect(stage == 5, "delete after height");
    stage = 6;
    ++delete_calls;
    return 0x333u;
}
unsigned char norrath_actors_a_view_44d0a8[4];
unsigned char norrath_actors_a_view_485358[4];
unsigned char norrath_factory_world_pointer_word[4];
unsigned char norrath_ctors_lava_wraith_view_449820[4];
static void set_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
static void run_case(unsigned initial_flags, int door, int height, bool present, bool want_delete) {
    stage = 0;
    delete_calls = 0;
    tag_present = present;
    scripted_door = door;
    scripted_height = height;
    alignas(16) unsigned char object[96];
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    std::memset(object, 0x5a, sizeof object);
    set_word(object + 12, initial_flags);
    set_word(norrath_factory_world_pointer_word, 0x20004444u);
    const unsigned result = norrath_construct_Switch(object, point, args);
    flags_at_door = word_at(object + 12);
    const unsigned during = present ? ((initial_flags | 0x00100003u) & 0xffefffffu)
                                    : (initial_flags | 0x00100003u);
    const unsigned final_flags = during | 0x00100000u;
    expect(result == final_flags, "return is the final flags word");
    expect(flags_at_door == final_flags, "stored flags match the return");
    expect(seen_map == nullptr && seen_arguments == args && seen_point != point, "null map and argument list");
    expect(std::memcmp(seen_bytes, point, 12) == 0, "point snapshot");
    expect(door_point[0] == 0x11 && std::memcmp(door_point + 1, point + 1, 11) == 0, "door point comes from the object");
    expect(door_world == reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x20004444u)), "world pointer");
    expect(height_world == door_world, "height reloads the world pointer");
    const int stored = static_cast<int>(static_cast<short>(door));
    expect(collision_index == stored && height_index == stored, "door index is the stored half");
    const unsigned half = unsigned(object[76]) | (unsigned(object[77]) << 8);
    const unsigned height_half = unsigned(object[78]) | (unsigned(object[79]) << 8);
    expect(half == (static_cast<unsigned>(door) & 0xffffu), "stored door half");
    expect(height_half == (static_cast<unsigned>(height) & 0xffffu), "stored height half");
    expect(word_at(object + 72) == static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_ctors_lava_wraith_view_449820)),
           "final view");
    expect(flags_during_door == during, "tag clear is visible to the door query");
    if (want_delete) expect(delete_calls == 1 && result != 0x333u, "delete does not become V0");
    else expect(delete_calls == 0 && stage == 5, "positive half skips delete");
    (void)during;
}
int main() {
    run_case(0u, 4, 9, false, false);
    run_case(0u, -2, 0x12345, true, true);
    run_case(0x00100000u, 0x18000, 3, true, true);
    run_case(0u, 7, 1, false, false);
    if (fails != 0) return 1;
    std::puts("switch constructor passed");
    return 0;
}
