#include "container_resources.hpp"
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
unsigned float_bits(float value) {
    return read_word(address(&value));
}
}
// __9ContainerG6Point3sPcN43iiiPPc. The two resource queries use the first name
// with the second name, then the first name with the third. Bit 2 of the third
// integer ORs 0x00100000 into the flags. V0 is the water result.
extern "C" unsigned norrath_construct_ContainerWithResources(
    void* object, const void* point, short type, char* first, char* second, char* third,
    char* fourth, char* fifth, int amount, int other, int bits, char** arguments) {
    PointBytes local = snapshot(point);
    (void)norrath_game_object_construct(object, &local, nullptr, arguments);
    const Address a = address(object);
    unsigned flags = read_word(a + 12u) | 0x10u;
    write_word(a + 120u, 0xffffffffu);
    write_word(a + 12u, flags);
    write_word(a + 104u, static_cast<unsigned>(bits));
    write_word(a + 72u, address(norrath_actors_a_view_44ccb8));
    write_half(a + 56u, static_cast<unsigned>(type));
    write_word(a + 84u, address(third));
    write_word(a + 88u, address(fourth));
    write_word(a + 92u, address(fifth));
    write_word(a + 76u, address(first));
    write_word(a + 80u, address(second));
    write_word(a + 116u, 0u);
    const unsigned first_resource = address(norrath_lump_find_resource(first, second));
    const unsigned second_resource = address(norrath_lump_find_resource(first, third));
    write_word(a + 96u, first_resource);
    write_word(a + 100u, second_resource);
    write_word(a + 32u, float_bits(static_cast<float>(other)));
    write_word(a + 28u, float_bits(static_cast<float>(amount)));
    if ((static_cast<unsigned>(bits) & 4u) != 0u) {
        flags |= 0x00100000u;
        write_word(a + 12u, flags);
    }
    PointBytes position = snapshot(pointer(a + 16u));
    const unsigned world = read_word(address(norrath_factory_world_pointer_word));
    const int water = norrath_world_check_for_water(pointer(world), &position, bits_to_float(0x41400000u), nullptr);
    bytes(a)[124] = static_cast<unsigned char>(water);
    return static_cast<unsigned>(water);
}
