#include "detail.hpp"
// __15PushPhysicsPropG6Point3 at 0x155ce8, 128 bytes.
// GameObject receives the point snapshot, a null map, and a null argument list.
// _vt$15PushPhysicsProp (0x448a70) is stored at object+72 and 0x10 is ORed into
// object+12 before the run-list call. The 12 bytes at object+16 are then copied
// to object+232. Low V0 is the word at object+16 after that copy, not the list result.
extern "C" unsigned norrath_construct_PushPhysicsPropSimple(void* object, const void* point) {
    using namespace norrath_actors_b;
    const PointBytes local = snapshot_point(point);
    (void)norrath_game_object_construct(object, &local, nullptr, nullptr);
    const Address a = address(object);
    write_word(a + 72u, address_bits(norrath_actors_b_view_448a70));
    write_word(a + 12u, read_word(a + 12u) | 0x10u);
    (void)norrath_actor_add_run_list(object);
    write_word(a + 232u, read_word(a + 16u));
    write_word(a + 236u, read_word(a + 20u));
    write_word(a + 240u, read_word(a + 24u));
    return read_word(a + 16u);
}
