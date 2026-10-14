#include "candle.hpp"
namespace {
using namespace norrath_actors_a;
struct PointBytes { unsigned x, y, z; };
static_assert(sizeof(PointBytes) == 12 && alignof(PointBytes) == 4, "raw point storage");
PointBytes snapshot(const void* point) {
    const Address a = address(point);
    return PointBytes{read_word(a), read_word(a + 4u), read_word(a + 8u)};
}
}
// __6CandleG6Point3i. lightAdd observes the first color/range words. The later
// half stores and byte 85 replace some of those fields before the run-list call.
extern "C" unsigned norrath_construct_Candle(void* object, const void* point, int kind) {
    PointBytes local = snapshot(point);
    (void)norrath_game_object_construct(object, &local, nullptr, nullptr);
    const Address a = address(object);
    const Address light = a + 76u;
    const unsigned tint = read_word(address(norrath_actors_a_view_481980));
    bytes(light)[9] = 0u;
    write_word(a + 124u, local.x);
    write_word(a + 128u, local.y);
    write_word(a + 132u, local.z);
    write_word(a + 72u, address(norrath_actors_a_view_44a9c0));
    write_word(light + 36u, 0x43700000u);
    write_word(light + 40u, tint);
    write_word(light + 44u, 0x3f800000u);
    write_word(light + 28u, 0x43700000u);
    write_word(light + 32u, tint);
    write_half(light + 18u, 200u);
    write_half(light + 20u, 150u);
    write_half(light + 22u, 50u);
    write_half(light + 10u, 200u);
    write_half(light + 12u, 150u);
    write_half(light + 14u, 50u);
    write_half(light + 16u, 0u);
    write_half(light + 24u, 0u);
    bytes(light)[8] = 0u;
    norrath_light_add(pointer(light));
    write_word(a + 12u, read_word(a + 12u) | 1u);
    bytes(a)[85] = 1u;
    bytes(a)[40] = 0xf0u;
    write_word(a + 144u, static_cast<unsigned>(kind));
    write_half(a + 94u, 51u);
    write_half(a + 96u, 39u);
    write_half(a + 98u, 18u);
    write_word(a + 136u, 0u);
    bytes(a)[140] = 0u;
    bytes(a)[141] = 0u;
    write_half(a + 100u, 0u);
    return norrath_actor_add_run_list(object);
}
