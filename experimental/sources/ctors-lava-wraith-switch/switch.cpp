#include "../ctors-lava-wraith-detail/detail.hpp"
// __6SwitchG6Point3PPc at 0x2126c0, 304 bytes.
// GameObject receives a point snapshot, a null resource map, and the argument
// list. _vt$6Switch is stored twice: 0x44d0a8, then 0x449820 after the door
// queries. Flags are ORed with 0x00100003. A present tag at 0x485358 clears
// bit 0x00100000 before those queries, and the last flag store sets that bit
// again. A negative door half calls objectAddToDeleteList. Low V0 is the
// final flags word, not the delete result.
extern "C" unsigned norrath_construct_Switch(void* object, const void* point, char** arguments) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_game_object_construct(object, &local, nullptr, arguments);
    const Address a = address(object);
    write_word(a + 72u, address_bits(norrath_actors_a_view_44d0a8));
    write_word(a + 12u, read_word(a + 12u) | 0x00100003u);
    if (norrath_actor_find_tag_string(object, reinterpret_cast<char*>(norrath_actors_a_view_485358), 0) != nullptr) {
        write_word(a + 12u, read_word(a + 12u) & 0xffefffffu);
    }
    const PointBytes position = snapshot_point(pointer(a + 16u));
    const int door = norrath_world_find_closest_door(world_pointer(), &position);
    write_half(a + 76u, static_cast<unsigned>(door));
    (void)norrath_world_set_door_collision(world_pointer(), low_signed(door), 1);
    const int height = norrath_world_get_door_height(world_pointer(), read_signed_half(a + 76u));
    write_half(a + 78u, static_cast<unsigned>(height));
    if (read_signed_half(a + 76u) < 0) {
        (void)norrath_object_add_to_delete_list(object);
    }
    const unsigned flags = read_word(a + 12u) | 0x00100000u;
    write_word(a + 12u, flags);
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_449820));
    return flags;
}
