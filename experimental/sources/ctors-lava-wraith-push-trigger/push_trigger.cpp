#include "../ctors-lava-wraith-detail/detail.hpp"
// __11PushTriggerG6Point3iPPc at 0x1d12d8, 404 bytes.
// GameObject receives a point snapshot, a null map and the argument list. View
// 0x44c710 goes to +72, flags gain 0x0001000b and the low half of the int goes
// to +56. A nonzero integer tag at 0x46cd20 clears flag bit 0. The string tag
// at 0x484fa0 selects the map-icon path: with no string the constructor ends
// and V0 is that null result. Otherwise the kind is 0 when the string equals
// the text at 0x484fa8, else 1 unless it equals the text at 0x46cd30 (kind 0
// again for that one). Four more string tags (0x46cd40..0x46cd70) are read
// in that order and addIcon receives the point snapshot, kind, string, the
// int and those four. Low V0 is addIcon's result.
extern "C" unsigned norrath_construct_PushTrigger(void* object, const void* point, int type, char** arguments) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_game_object_construct(object, &local, nullptr, arguments);
    const Address a = address(object);
    write_word(a + 12u, read_word(a + 12u) | 0x0001000bu);
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44c710));
    write_half(a + 56u, static_cast<unsigned>(type));
    if (norrath_game_object_find_tag_int(object, reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_46cd20), 0) != 0) {
        write_word(a + 12u, read_word(a + 12u) & 0xfffffffeu);
    }
    char* const name = norrath_actor_find_tag_string(object, reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_484fa0), 0);
    if (name == nullptr) return 0u;
    int kind = 0;
    if (strcmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_484fa8)) != 0) {
        kind = strcmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_46cd30)) != 0 ? 1 : 0;
    }
    char* const first = norrath_actor_find_tag_string(object, reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_46cd40), 0);
    char* const second = norrath_actor_find_tag_string(object, reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_46cd50), 0);
    char* const third = norrath_actor_find_tag_string(object, reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_46cd60), 0);
    char* const fourth = norrath_actor_find_tag_string(object, reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_46cd70), 0);
    return norrath_map_add_icon(&local, kind, name, type, first, second, third, fourth);
}
