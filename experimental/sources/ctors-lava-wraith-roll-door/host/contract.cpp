#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../roll_door.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
using namespace hs;
static bool tag_present;
static int scripted_door, scripted_height;
static unsigned char* obj;
unsigned char norrath_actors_a_view_44d0a8[4], norrath_actors_a_view_485358[4];
unsigned char norrath_factory_world_pointer_word[4];
unsigned char norrath_ctors_lava_wraith_view_42c630[4], norrath_ctors_lava_wraith_view_449940[4];
static void* const the_world = reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x20004444u));
extern "C" unsigned norrath_game_object_construct(void* object, const void* point, void* map, char** arguments) {
    event("GameObject map=%08x args=%d p=%02x", low(map), arguments != nullptr, ((const unsigned char*)point)[0]);
    obj = (unsigned char*)object;
    std::memcpy(obj + 16, point, 12);
    obj[16] = 0x21;
    return 1;
}
extern "C" char* norrath_actor_find_tag_string(void*, char* tag, int n) {
    event("tag %08x %d flags=%08x", low(tag), n, word_at(obj + 12));
    return tag_present ? tag : nullptr;
}
extern "C" int norrath_world_find_closest_door(void* w, const void* p) {
    event("closest %d p=%02x flags=%08x", w == the_world, ((const unsigned char*)p)[0], word_at(obj + 12));
    return scripted_door;
}
extern "C" int norrath_world_set_door_collision(void* w, int i, int v) {
    event("collide %d %d %d", w == the_world, i, v);
    return 0x700 + i;
}
extern "C" int norrath_world_get_door_height(void* w, int i) { event("height %d %d", w == the_world, i); return scripted_height; }
extern "C" unsigned norrath_object_add_to_delete_list(void*) { event("delete"); return 9; }
static void run_case(unsigned flags0, int door, int height, bool present) {
    calls.clear();
    tag_present = present; scripted_door = door; scripted_height = height;
    Image<96> o, want;
    o.w(12, flags0); want.w(12, flags0);
    put(norrath_factory_world_pointer_word, 0x20004444u, 4);
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    const unsigned r = norrath_construct_RollDoor(o.b, point, (short)0xfff9, args);
    const int s = (short)door;
    const unsigned flags_mid = present ? ((flags0 | 0x100003u) & 0xffefffffu) : (flags0 | 0x100003u);
    std::vector<std::string> w;
    char b[128];
    std::snprintf(b, sizeof b, "GameObject map=%08x args=1 p=01", low(norrath_ctors_lava_wraith_view_42c630)); w.push_back(b);
    std::snprintf(b, sizeof b, "tag %08x 0 flags=%08x", low(norrath_actors_a_view_485358), flags0 | 0x100003u); w.push_back(b);
    std::snprintf(b, sizeof b, "closest 1 p=21 flags=%08x", flags_mid); w.push_back(b);
    std::snprintf(b, sizeof b, "collide 1 %d 1", s); w.push_back(b);
    std::snprintf(b, sizeof b, "height 1 %d", s); w.push_back(b);
    if (s < 0) w.push_back("delete");
    std::snprintf(b, sizeof b, "collide 1 %d 1", s); w.push_back(b);
    expect_log(w, "roll door");
    want.w(16, 0x21, 1); for (int i = 1; i < 12; ++i) want.b[16 + i] = point[i];
    want.w(72, low(norrath_ctors_lava_wraith_view_449940));
    want.w(12, flags_mid & ~2u);
    want.w(76, (unsigned)door, 2); want.w(78, (unsigned)height, 2);
    want.w(40, 72, 1); want.w(41, 84, 1); want.w(56, 0xfff9, 2);
    expect_image(o, want, "roll door image");
    expect(r == 0x700u + (unsigned)s, "return is the last collision result");
}
int main() {
    run_case(0, 5, 0x1234, false);
    run_case(0x00100000u, -3, 0x12345, true);
    run_case(6, 0x18000, 1, true);
    return finish("roll door constructor");
}
