#include "detail.hpp"
// __10HateBridgePcG6Point3sPPc at 0x2263e8, 124 bytes.
// After AutoProp: _vt$10HateBridge (0x44eeb0) at object+72, 1 ORed into
// object+12, setPassableState__10HateBridgeb(object, 1), objectAddToRunList,
// then byte 40 stores the low 8 bits of -80. Low V0 is the run-list result.
extern "C" unsigned norrath_construct_HateBridge(void* object, char* name, const void* point,
                                                short type, char** arguments) {
    using namespace norrath_actors_b;
    const PointBytes local = snapshot_point(point);
    (void)norrath_construct_AutoProp(object, name, &local, type, arguments);
    const Address a = address(object);
    write_word(a + 72u, address_bits(norrath_actors_b_view_44eeb0));
    write_word(a + 12u, read_word(a + 12u) | 1u);
    (void)norrath_hate_bridge_set_passable_state(object, 1);
    const unsigned result = norrath_actor_add_run_list(object);
    bytes(a)[40] = static_cast<unsigned char>(-80);
    return result;
}
