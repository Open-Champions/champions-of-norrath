#include "detail.hpp"
// __7TriggerG6Point3PPc at 0x1d1470, 256 bytes.
// GameObject receives the point snapshot, a null map, and the argument list.
// _vt$7Trigger (0x44c8c0) is stored at object+72. Two findTagInt results are
// converted with cvt.s.w; the second is negated with negu first. Object+12 is
// replaced with 3. findTagString's pointer is stored at object+92. The sums of
// object+16/+20 with those converted tags are bounded with min.s/max.s into
// words 76, 80, 84, and 88 before the medium-run-list call. Word 104 is then -1.
// Low V0 is the medium-run-list result. Tag addresses are 0x484fd8, 0x484fe0,
// and 0x484fe8. NaN and signed-zero min/max remain unknown.
extern "C" unsigned norrath_construct_Trigger(void* object, const void* point, char** arguments) {
    using namespace norrath_actors_b;
    const PointBytes local = snapshot_point(point);
    (void)norrath_game_object_construct(object, &local, nullptr, arguments);
    const Address a = address(object);
    write_word(a + 72u, address_bits(norrath_actors_b_view_44c8c0));
    const int width = norrath_game_object_find_tag_int(object, reinterpret_cast<char*>(norrath_actors_b_view_484fd8), 0);
    const float width_single = static_cast<float>(width);
    const int height = norrath_game_object_find_tag_int(object, reinterpret_cast<char*>(norrath_actors_b_view_484fe0), 0);
    const unsigned negated = 0u - static_cast<unsigned>(height);
    write_word(a + 12u, 3u);
    const float height_single = static_cast<float>(signed_word(negated));
    char* tag = norrath_actor_find_tag_string(object, reinterpret_cast<char*>(norrath_actors_b_view_484fe8), 0);
    const float x = read_float(a + 16u);
    const float y = read_float(a + 20u);
    const float sum_x = x + width_single;
    const float sum_y = y + height_single;
    write_word(a + 92u, address_bits(tag));
    write_float(a + 76u, minimum_single(x, sum_x));
    write_float(a + 80u, minimum_single(y, sum_y));
    write_float(a + 84u, maximum_single(x, sum_x));
    write_float(a + 88u, maximum_single(y, sum_y));
    const unsigned result = norrath_actor_add_medium_run_list(object);
    write_word(a + 104u, 0xffffffffu);
    return result;
}
