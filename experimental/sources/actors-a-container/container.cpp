#include "container.hpp"
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
}
// __9ContainerG6Point3sPPc. The water query reads the object position after the
// base call, not the incoming point. V0 remains the water result; byte 124 keeps
// only its low 8 bits.
extern "C" unsigned norrath_construct_Container(void* object, const void* point, short type,
                                                char** arguments) {
    PointBytes local = snapshot(point);
    (void)norrath_game_object_construct(object, &local, nullptr, arguments);
    const Address a = address(object);
    const unsigned flags = read_word(a + 12u) | 0x10u;
    write_word(a + 72u, address(norrath_actors_a_view_44ccb8));
    write_half(a + 56u, static_cast<unsigned>(type));
    write_word(a + 120u, 0xffffffffu);
    write_word(a + 12u, flags);
    write_word(a + 116u, 0u);
    PointBytes position = snapshot(pointer(a + 16u));
    const unsigned world = read_word(address(norrath_factory_world_pointer_word));
    const int water = norrath_world_check_for_water(pointer(world), &position, bits_to_float(0x41400000u), nullptr);
    bytes(a)[124] = static_cast<unsigned char>(water);
    return static_cast<unsigned>(water);
}
