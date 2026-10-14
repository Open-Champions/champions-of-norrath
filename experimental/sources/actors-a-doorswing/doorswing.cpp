#include "doorswing.hpp"
namespace {
using namespace norrath_actors_a;
struct PointBytes { unsigned x, y, z; };
static_assert(sizeof(PointBytes) == 12 && alignof(PointBytes) == 4, "raw point storage");
PointBytes snapshot(const void* point) {
    const Address a = address(point);
    return PointBytes{read_word(a), read_word(a + 4u), read_word(a + 8u)};
}
int low_signed(int value) {
    return static_cast<int>(static_cast<short>(value));
}
}
// __9DoorSwingG6Point3PPc. Unlike DoorSecret, a present tag string leaves bit
// 0x00100000 clear. A nonzero integer tag adds the object to the delete list,
// and that call's result is V0. A zero integer tag leaves V0 at 0.
extern "C" unsigned norrath_construct_DoorSwing(void* object, const void* point, char** arguments) {
    PointBytes local = snapshot(point);
    (void)norrath_game_object_construct(object, &local, nullptr, arguments);
    const Address a = address(object);
    unsigned flags = read_word(a + 12u) | 0x00100003u;
    write_word(a + 72u, address(norrath_actors_a_view_44d0a8));
    write_word(a + 12u, flags);
    if (norrath_actor_find_tag_string(object, reinterpret_cast<char*>(norrath_actors_a_view_485358), 0) != nullptr) {
        flags &= 0xffefffffu;
        write_word(a + 12u, flags);
    }
    PointBytes position = snapshot(pointer(a + 16u));
    const int door = norrath_world_find_closest_door(
        pointer(read_word(address(norrath_factory_world_pointer_word))), &position);
    write_half(a + 76u, static_cast<unsigned>(door));
    const int index = low_signed(door);
    (void)norrath_world_set_door_collision(
        pointer(read_word(address(norrath_factory_world_pointer_word))), index, 1);
    const int height = norrath_world_get_door_height(
        pointer(read_word(address(norrath_factory_world_pointer_word))), read_signed_half(a + 76u));
    write_half(a + 78u, static_cast<unsigned>(height));
    if (read_signed_half(a + 76u) < 0) {
        (void)norrath_object_add_to_delete_list(object);
    }
    write_word(a + 72u, address(norrath_actors_a_view_4499d8));
    (void)norrath_world_set_door_collision(
        pointer(read_word(address(norrath_factory_world_pointer_word))), read_signed_half(a + 76u), 1);
    const int tag = norrath_game_object_find_tag_int(
        object, reinterpret_cast<char*>(norrath_actors_a_view_485360), 0);
    if (tag != 0) {
        return norrath_object_add_to_delete_list(object);
    }
    return static_cast<unsigned>(tag);
}
