#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int tags;
static char* seen_tags[3];
static char marker[] = "tag";
extern "C" unsigned norrath_game_object_construct(void* object, const void* point, void* resource_map, char** arguments) {
    (void)point; (void)resource_map; (void)arguments;
    auto* raw = reinterpret_cast<unsigned char*>(object);
    const float x = 10.0f, y = 4.0f;
    std::memcpy(raw + 16, &x, 4);
    std::memcpy(raw + 20, &y, 4);
    raw[12] = 0x99;
    return 0u;
}
extern "C" int norrath_game_object_find_tag_int(void* object, char* tag, int occurrence) {
    (void)object;
    expect(occurrence == 0, "occurrence zero");
    seen_tags[tags++] = tag;
    return tags == 1 ? 4 : 7;
}
extern "C" char* norrath_actor_find_tag_string(void* object, char* tag, int occurrence) {
    (void)object;
    expect(occurrence == 0 && tags == 2, "string tag after both ints");
    auto* raw = reinterpret_cast<unsigned char*>(object);
    unsigned flags = unsigned(raw[12]) | (unsigned(raw[13]) << 8) | (unsigned(raw[14]) << 16) | (unsigned(raw[15]) << 24);
    expect(flags == 3u, "flags replaced with 3 before string tag");
    seen_tags[tags++] = tag;
    return marker;
}
extern "C" unsigned norrath_actor_add_medium_run_list(void* object) {
    auto* raw = reinterpret_cast<unsigned char*>(object);
    float values[4];
    std::memcpy(values, raw + 76, 16);
    expect(values[0] == 10.0f && values[1] == -3.0f && values[2] == 14.0f && values[3] == 4.0f, "bounds before medium list");
    unsigned tag = unsigned(raw[92]) | (unsigned(raw[93]) << 8) | (unsigned(raw[94]) << 16) | (unsigned(raw[95]) << 24);
    expect(tag == static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(marker)), "string pointer stored");
    expect(raw[104] == 0x5a, "word 104 not written yet");
    return 0x777u;
}
unsigned char norrath_actors_b_view_44c8c0[4];
unsigned char norrath_actors_b_view_484fd8[4];
unsigned char norrath_actors_b_view_484fe0[4];
unsigned char norrath_actors_b_view_484fe8[4];
int main() {
    alignas(16) unsigned char object[112];
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    std::memset(object, 0x5a, sizeof object);
    const unsigned result = norrath_construct_Trigger(object, point, args);
    expect(result == 0x777u, "forwards medium-list result");
    expect(tags == 3, "three tag queries");
    expect(seen_tags[0] == reinterpret_cast<char*>(norrath_actors_b_view_484fd8), "first tag");
    expect(seen_tags[1] == reinterpret_cast<char*>(norrath_actors_b_view_484fe0), "second tag");
    expect(seen_tags[2] == reinterpret_cast<char*>(norrath_actors_b_view_484fe8), "string tag");
    unsigned end = unsigned(object[104]) | (unsigned(object[105]) << 8) | (unsigned(object[106]) << 16) | (unsigned(object[107]) << 24);
    expect(end == 0xffffffffu, "word 104 set after medium list");
    if (fails != 0) return 1;
    std::puts("trigger constructor passed");
    return 0;
}
