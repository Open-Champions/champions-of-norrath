#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int stage;
static long long state_result;
static char first_lump[] = "lumpa", first_name[] = "resa", second_name[] = "resb", third_name[] = "resc", fourth_name[] = "resd";
static char* lump_args[4];
static int lump_calls;
static int destroy_calls;
static void* destroy_object;
static int destroy_flag;
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
unsigned char norrath_ctors_ant_lantern_view_44cc28[16];
unsigned char norrath_ctors_ant_lantern_view_46d070[4];
extern "C" unsigned norrath_game_object_construct(void* object, const void* point, void* map, char** args) {
    expect(stage == 0 && map == nullptr && args == nullptr && point != nullptr, "game object first, null arguments");
    stage = 1;
    std::memset(object, 0, 128);
    return 0;
}
extern "C" long long norrath_script_get_game_state(char* name) {
    expect(stage == 1 && name == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_46d070), "state name");
    stage = 2;
    return state_result;
}
extern "C" void* norrath_lump_find_resource(char* lump, char* resource) {
    expect(stage == 2 || stage == 3, "lookups after the state check");
    stage = 3;
    lump_args[lump_calls * 2] = lump;
    lump_args[lump_calls * 2 + 1] = resource;
    return lump_calls++ == 0 ? static_cast<void*>(third_name) : static_cast<void*>(fourth_name);
}
extern "C" unsigned norrath_actor_add_run_list(void* object) {
    expect(stage == 3, "run list last");
    stage = 4;
    const unsigned char* o = static_cast<unsigned char*>(object);
    expect(word_at(o + 28) == 0x43480000u && word_at(o + 32) == 0x43480000u && o[40] == 120, "singles and byte 40");
    return 0x7117u;
}
static unsigned fake_destroy(void* object, int flag) {
    ++destroy_calls; destroy_object = object; destroy_flag = flag;
    return 0xde1u;
}
int main() {
    alignas(16) unsigned char object[256];
    unsigned char point[12] = {0};
    std::memset(object, 0, sizeof object);
    norrath_ctors_ant_lantern::host_note(first_lump);
    norrath_ctors_ant_lantern::host_note(second_name);
    norrath_ctors_ant_lantern::host_note(object);
    state_result = 0;
    unsigned r = norrath_construct_Ice(object, point, static_cast<short>(-3), first_lump, first_name, second_name, third_name, fourth_name);
    expect(r == 0x7117u && stage == 4 && lump_calls == 2, "run list result and two lookups");
    expect(lump_args[0] == first_lump && lump_args[1] == first_name, "first lookup uses arguments one and two");
    expect(lump_args[2] == first_lump && lump_args[3] == second_name, "second lookup reloads words 76 and 84");
    const auto bits = [](const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); };
    expect(word_at(object + 76) == bits(first_lump) && word_at(object + 80) == bits(first_name) &&
           word_at(object + 84) == bits(second_name) && word_at(object + 88) == bits(third_name) && word_at(object + 92) == bits(fourth_name),
           "argument words");
    expect(word_at(object + 96) == bits(third_name) && word_at(object + 100) == bits(fourth_name), "lookup results");
    expect(object[56] == 0xfd && object[57] == 0xff && word_at(object + 12) == 9u, "type half and flags");
    expect(word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_44cc28), "vtable");
    // delete path: vtable+8 = short adjustment, vtable+12 = function pointer
    stage = 0; lump_calls = 0;
    state_result = 0x100000000ll;   // nonzero only in the high word still counts
    norrath_ctors_ant_lantern::host_note(norrath_ctors_ant_lantern_view_44cc28);
    const unsigned fn = bits(reinterpret_cast<void*>(+fake_destroy));
    norrath_ctors_ant_lantern::host_note(reinterpret_cast<void*>(+fake_destroy));
    norrath_ctors_ant_lantern_view_44cc28[8] = 0x10; norrath_ctors_ant_lantern_view_44cc28[9] = 0;
    std::memcpy(norrath_ctors_ant_lantern_view_44cc28 + 12, &fn, 4);
    std::memset(object, 0, sizeof object);
    r = norrath_construct_Ice(object, point, 5, first_lump, first_name, second_name, third_name, fourth_name);
    expect(r == 0xde1u && destroy_calls == 1 && destroy_flag == 3 && destroy_object == object + 16, "virtual delete with adjustment 16 and flag 3");
    expect(lump_calls == 0 && object[56] == 0 && word_at(object + 12) == 0, "no stores after delete");
    if (fails != 0) return 1;
    std::puts("ice constructor passed");
    return 0;
}
