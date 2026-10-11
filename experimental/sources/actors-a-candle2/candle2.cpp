#include "candle2.hpp"
namespace {
using namespace norrath_actors_a;
struct PointBytes { unsigned x, y, z; };
static_assert(sizeof(PointBytes) == 12 && alignof(PointBytes) == 4, "raw point storage");
PointBytes snapshot(const void* point) {
    const Address a = address(point);
    return PointBytes{read_word(a), read_word(a + 4u), read_word(a + 8u)};
}
}
// __7Candle2G6Point3. Both base-call pointers are cleared. V0 is the run-list result.
extern "C" unsigned norrath_construct_Candle2(void* object, const void* point) {
    PointBytes local = snapshot(point);
    (void)norrath_game_object_construct(object, &local, nullptr, nullptr);
    const Address a = address(object);
    const unsigned flags = read_word(a + 12u) | 1u;
    write_word(a + 72u, address(norrath_actors_a_view_44a930));
    bytes(a)[40] = 0xf0u;
    write_word(a + 12u, flags);
    bytes(a)[76] = 0u;
    bytes(a)[77] = 0u;
    return norrath_actor_add_run_list(object);
}
