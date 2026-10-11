#include "blocker.hpp"
namespace {
using namespace norrath_actors_a;
struct PointBytes { unsigned x, y, z; };
static_assert(sizeof(PointBytes) == 12 && alignof(PointBytes) == 4, "raw point storage");
PointBytes snapshot(const void* point) {
    const Address a = address(point);
    return PointBytes{read_word(a), read_word(a + 4u), read_word(a + 8u)};
}
float bits_to_float(unsigned bits) {
    float value;
    write_word(address(&value), bits);
    return value;
}
unsigned float_to_bits(float value) { return read_word(address(&value)); }
}
// __7BlockerG6Point3sPPc. The base call keeps the incoming argument list in A3.
// A zero height takes the 0x205 flag update before the common 0x2 update, and
// V0 stays the value from before that last update.
extern "C" unsigned norrath_construct_Blocker(void* object, const void* point, short type,
                                              char** arguments) {
    PointBytes local = snapshot(point);
    (void)norrath_game_object_construct(object, &local, nullptr, arguments);
    const Address a = address(object);
    write_word(a + 72u, address(norrath_actors_a_view_44c830));
    write_word(a + 28u, 0x41c00000u);
    write_half(a + 56u, static_cast<unsigned>(type));
    char* first_tag = text(address(norrath_actors_a_view_485400));
    if (norrath_actor_find_tag_string(object, first_tag, 0) != nullptr) {
        const int value = norrath_game_object_find_tag_int(object, first_tag, 0);
        write_word(a + 28u, float_to_bits(static_cast<float>(value)));
    }
    char* second_tag = text(address(norrath_actors_a_view_485408));
    if (norrath_actor_find_tag_string(object, second_tag, 0) != nullptr) {
        const int value = norrath_game_object_find_tag_int(object, second_tag, 0);
        write_word(a + 28u, float_to_bits(static_cast<float>(value)));
    }
    const int third = norrath_game_object_find_tag_int(object, text(address(norrath_actors_a_view_470830)), 0);
    unsigned result = static_cast<unsigned>(third);
    if (third != 0) {
        write_word(a + 32u, 0x3f800000u);
        result = read_word(a + 12u) | 0x10u;
        write_word(a + 12u, result);
    }
    if (bits_to_float(read_word(a + 28u)) == 0.0f) {
        result = read_word(a + 12u) | 0x205u;
        write_word(a + 12u, result);
    }
    write_word(a + 12u, read_word(a + 12u) | 2u);
    return result;
}
