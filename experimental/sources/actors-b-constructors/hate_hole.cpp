#include "detail.hpp"
// __8HateHolePcG6Point3sPPc at 0x226298, 128 bytes.
// After AutoProp: _vt$8HateHole (0x44ef58) at object+72 and 1 ORed into object+12.
// worldGetDoorHeight receives the pointer loaded from `world` (0x4844a0) and
// the word at object+396. Its signed result is converted with cvt.s.w and
// stored at object+408, then objectAddToRunList. Low V0 is that list result.
extern "C" unsigned norrath_construct_HateHole(void* object, char* name, const void* point,
                                              short type, char** arguments) {
    using namespace norrath_actors_b;
    const PointBytes local = snapshot_point(point);
    (void)norrath_construct_AutoProp(object, name, &local, type, arguments);
    const Address a = address(object);
    write_word(a + 72u, address_bits(norrath_actors_b_view_44ef58));
    write_word(a + 12u, read_word(a + 12u) | 1u);
    const int index = signed_word(read_word(a + 396u));
    const int height = norrath_world_get_door_height(pointer(read_word(address(norrath_actors_b_view_4844a0))), index);
    write_float(a + 408u, static_cast<float>(height));
    return norrath_actor_add_run_list(object);
}
