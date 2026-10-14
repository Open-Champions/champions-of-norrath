#include "../ctors-lava-wraith-detail/detail.hpp"
// __8RollDoorG6Point3sPPc at 0x215a78, 352 bytes.
// GameObject receives a point snapshot, the resource map at 0x42c630, and the
// argument list. The WorldPart view 0x44d0a8 is stored first, flags are ORed
// with 0x00100003, and a present tag at 0x485358 clears bit 0x00100000. The
// closest door half goes to +76 and its height to +78. A negative half calls
// objectAddToDeleteList. The half is then reloaded unsigned, bytes 40/41 get
// 'H' and 'T', bit 2 of the flags is cleared, the sign-extended type is stored
// at +56, view 0x449940 replaces the first view, and the collision update
// repeats with the sign-extended half. Low V0 is that second update's result.
extern "C" unsigned norrath_construct_RollDoor(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_game_object_construct(object, &local, norrath_ctors_lava_wraith_view_42c630, arguments);
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
    const unsigned half = static_cast<unsigned>(read_signed_half(a + 76u)) & 0xffffu;
    write_byte(a + 40u, 72u);
    write_byte(a + 41u, 84u);
    const unsigned flags = read_word(a + 12u) & 0xfffffffdu;
    write_half(a + 56u, static_cast<unsigned>(type));
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_449940));
    write_word(a + 12u, flags);
    return static_cast<unsigned>(norrath_world_set_door_collision(world_pointer(), low_signed(static_cast<int>(half)), 1));
}
